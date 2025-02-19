#include "UnitMovementComponent.h"
#include "Emerald_MRVR/Components/Unit/Combat/CombatComponent.h"
#include "EngineUtils.h"
#include "SplineComponent.h"
#include "Emerald_MRVR/Support/EmeraldBlueprintFunctionLibrary.h"
#include "Emerald_MRVR/Actors/MilitaryBase/MilitaryStation.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Net/UnrealNetwork.h"


UUnitMovementComponent::UUnitMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	SetIsReplicated(true);
	bAutoActivate = true;
}

void UUnitMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUnitMovementComponent, UnitSpeed);
}

void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Unit = Cast<AUnit>(GetOwner());
	if (Unit)
	{
		UnitSpeed = Unit->Speed;
	}
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bMovementEnabled)
	{
		MoveAlongPath(DeltaTime);
	}

	if (bIsRestartingMovement)
	{
		Accelerate(DeltaTime);
	}
}

void UUnitMovementComponent::FindPathPoints()
{
	if (!Unit) return;
	
	PathPoints = UEmeraldBlueprintFunctionLibrary::SortPathPoints(this, PathPointClass, Unit->bIsReversed);
}

void UUnitMovementComponent::CreateMovementPath()
{
	FVector Start = Unit->GetActorLocation();
	FVector End = FVector::ZeroVector;

	UWorld* World = GetWorld();
	if (!World) return;

	// Najít soupeřovu základnu
	AMilitaryStation* EnemyStation = nullptr;
	for (TActorIterator<AMilitaryStation> It(World); It; ++It)
	{
		AMilitaryStation* MilitaryBase = *It;
		if (MilitaryBase && MilitaryBase->GetOwner() != GetOwner()->GetOwner()) // Ověříme, že to není naše základna
		{
			EnemyStation = MilitaryBase;
			break; // Jakmile najdeme soupeře, přestaneme hledat
		}
	}

	// Pokud jsme našli soupeře, nastavíme End podle typu jednotky
	if (EnemyStation)
	{
 		if (bIsFlying) // Pokud je jednotka létající
		{
			End = EnemyStation->SpawnPoint_Air->GetComponentLocation();
		}
		else // Pozemní jednotka
		{
			End = EnemyStation->SpawnPoint_Ground->GetComponentLocation();
		}
	}

	// Ověříme, že jsme našli platný EndPoint
	if (End == FVector::ZeroVector) return;
	
	MovementSpline = UEmeraldBlueprintFunctionLibrary::CreateSplinePath(this, Start, End, PathPoints,GetOwner());
}

void UUnitMovementComponent::MoveAlongPath(float DeltaTime)
{
	if (!MovementSpline || !Unit) return;

	// Posun po spline
	SplineDistance += (bIsReversedMovement ? -UnitSpeed : UnitSpeed) * DeltaTime;
	SplineDistance = FMath::Clamp(SplineDistance, 0.0f, MovementSpline->GetSplineLength());

	// Získání nové pozice na spline
	FVector NewLocation = MovementSpline->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);

	// Směrový vektor (kam jednotka míří)
	FVector ForwardVector = MovementSpline->GetDirectionAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);

	// Přidání sklonu (např. -20° dolů)
	FRotator TargetRotation = ForwardVector.Rotation();
	TargetRotation.Pitch += -2.0f; // Udržení mírného předklonu

	// Výpočet náklonu křídel (Roll)
	FVector FutureDirection = MovementSpline->GetDirectionAtDistanceAlongSpline(SplineDistance + 10.0f, ESplineCoordinateSpace::World);
	float Curvature = FVector::CrossProduct(ForwardVector, FutureDirection).Z;

	// Invertujeme, aby letadlo naklánělo křídla správně
	float RollAmount = RollAmountMultiplier * FMath::Pow(FMath::Abs(Curvature), 1.5f) * FMath::Sign(Curvature);
	TargetRotation.Roll = RollAmount; 

	// Pokud je jednotka létající, ponecháme původní Yaw (nebude se otáčet okolo osy)
	if (Unit->bIsFlyingUnit)
	{
		 TargetRotation.Pitch += -8.0f;
	}

	// Plynulá interpolace rotace
	FRotator SmoothRotation = FMath::RInterpTo(GetOwner()->GetActorRotation(), TargetRotation, DeltaTime, SmoothRotationInterpSpeed);

	// Aplikace nové rotace a pozice
	Unit->SetActorRotation(SmoothRotation);
	Unit->SetActorLocation(NewLocation);
}

void UUnitMovementComponent::Turn180()
{
	FRotator BackwardRotation = GetOwner()->GetActorRotation() + FRotator(0, 180, 0);
	GetOwner()->SetActorRotation(BackwardRotation);
	bIsReversedMovement = !bIsReversedMovement;
}

void UUnitMovementComponent::TurnRandom()
{
	float RandYaw = FMath::RandRange(-90, 90);
	float RandPitch = FMath::RandRange(10, 45);
	FRotator RandomTurn = GetOwner()->GetActorRotation() + FRotator(RandPitch, RandYaw, 0);
	GetOwner()->SetActorRotation(RandomTurn);
	// Add destroy timer or return Location;
}

void UUnitMovementComponent::StopUnit()
{
	bMovementEnabled = false;
}

void UUnitMovementComponent::Accelerate(float DeltaTime)
{
	// Restart Movement
	if (!Unit)
	{
		return;
	}
	FVector ForwardDirection = Unit->GetActorForwardVector();
	StartingSpeed += DeltaTime * Acceleration;
	FVector ForwardMovement = ForwardDirection * (StartingSpeed * DeltaTime); // *??
	FVector DeltaLocation = ForwardMovement;
	if (StartingSpeed < UnitSpeed)
	{
		Unit->AddActorWorldOffset(DeltaLocation);
	}
	else
	{
		UCombatComponent* CombatComponent = Unit->FindComponentByClass<UCombatComponent>();
		if (CombatComponent)
		{
			GetWorld()->GetTimerManager().ClearTimer(CombatComponent->FightSequenceHandle);
			bIsRestartingMovement = false;
			bMovementEnabled = true;
			
		}
	}
}

void UUnitMovementComponent::RestartMovement()
{
	bIsRestartingMovement = true;
}

