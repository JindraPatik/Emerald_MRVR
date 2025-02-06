#include "UnitMovementComponent.h"
#include "CombatComponent.h"
#include "EngineUtils.h"
#include "MilitaryBaseComp.h"
#include "SplineComponent.h"
#include "Emerald_MRVR/EK_BlueprintFunctionLbrary.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/BasePawn.h"
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

	/*FindPathPoints();
	CreateMovementPath();*/
	
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bMovementEnabled)
	{
		MoveTo(DeltaTime);
		// MoveAlongPath(DeltaTime);
	}

	if (bIsRestartingMovement)
	{
		Accelerate(DeltaTime);
	}
}

void UUnitMovementComponent::MoveTo(float DeltaTime) const
{
	if (!Unit)
	{
		return;
	}

	if (bMovementEnabled)
	{
		FVector ForwardDirection = Unit->GetActorForwardVector(); // Směr vpřed
		ForwardDirection.Normalize();

		// Konstantní rychlost vpřed
		FVector ForwardMovement = ForwardDirection * (UnitSpeed * DeltaTime);

		// Oscilace do stran a nahoru/dolů
		UWorld* World = GetWorld();
		if (World)
		{
			float Time = GetWorld()->GetTimeSeconds(); // Aktuální čas ve hře
			float SidewaysOscillation = FMath::Sin(Time * OscillationFrequency) * OscillationAmplitudeX; // Oscilace do stran
			float VerticalOscillation = FMath::Sin(Time * OscillationFrequency * 0.5f) * OscillationAmplitudeZ; // Oscilace nahoru/dolů

			FVector OscillationOffset = FVector(SidewaysOscillation, 0.f, VerticalOscillation);

			// Spočítání celkového pohybu
			FVector DeltaLocation = ForwardMovement + OscillationOffset;
			Unit->AddActorWorldOffset(DeltaLocation);

			// Simulace Roll na základě pohybu do stran
			float TargetRoll = SidewaysOscillation * RollFactor;
			FRotator CurrentRotation = Unit->GetActorRotation();
			FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, TargetRoll);

			// Nastavení nové rotace
			Unit->SetActorRotation(NewRotation);
		}
	}
}

/*void UUnitMovementComponent::FindPathPoints()
{
	if (!Unit) return;
	
	PathPoints = UEK_BlueprintFunctionLbrary::SortPathPoints(this, PathPointClass, Unit->bIsReversed);
}*/

/*void UUnitMovementComponent::CreateMovementPath()
{
	FVector Start = Unit->UnitSpawnLocation;
	FVector End = FVector::ZeroVector;

	UWorld* World = GetWorld();
	if (!World) return;

	// Najít soupeřovu základnu
	AMilitaryBase* EnemyBase = nullptr;
	for (TActorIterator<AMilitaryBase> It(World); It; ++It)
	{
		AMilitaryBase* MilitaryBase = *It;
		if (MilitaryBase && MilitaryBase->GetOwner() != GetOwner()->GetOwner()) // Ověříme, že to není naše základna
		{
			EnemyBase = MilitaryBase;
			break; // Jakmile najdeme soupeře, přestaneme hledat
		}
	}

	// Pokud jsme našli soupeře, nastavíme End podle typu jednotky
	if (EnemyBase)
	{
		if (Unit->bIsFlyingUnit) // Pokud je jednotka létající
		{
			End = EnemyBase->SpawnPoint_Air->GetComponentLocation();
		}
		else // Pozemní jednotka
		{
			End = EnemyBase->SpawnPoint_Ground->GetComponentLocation();
		}
	}

	// Ověříme, že jsme našli platný EndPoint
	if (End == FVector::ZeroVector) return;
	
	MovementSpline = UEK_BlueprintFunctionLbrary::CreateSplinePath(this, Start, End, PathPoints, GetOwner());
}*/

/*void UUnitMovementComponent::MoveAlongPath(float DeltaTime)
{
	if (!MovementSpline) return;

	SplineDistance += UnitSpeed * DeltaTime;

	FVector NewLocation = MovementSpline->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);
	Unit->SetActorLocation(NewLocation);
	
}*/

void UUnitMovementComponent::Turn180()
{
	FRotator BackwardRotation = GetOwner()->GetActorRotation() + FRotator(0, 180, 0);
	GetOwner()->SetActorRotation(BackwardRotation);
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

