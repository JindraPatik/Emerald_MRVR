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

	// Find Enemy Military Station
	AMilitaryStation* EnemyBase = nullptr;
	for (TActorIterator<AMilitaryStation> It(World); It; ++It)
	{
		AMilitaryStation* MilitaryBase = *It;
		if (MilitaryBase && MilitaryBase->GetOwner() != GetOwner()->GetOwner()) // Check if it isn't Player's Station
		{
			EnemyBase = MilitaryBase;
			break; // If found, break
		}
	}

	// Set End to Enemy Station
	if (EnemyBase)
	{
 		if (bIsFlying)
		{
			End = EnemyBase->SpawnPointAir->GetComponentLocation();
		}
		else
		{
			End = EnemyBase->SpawnPointGround->GetComponentLocation();
		}
	}

	if (End == FVector::ZeroVector) return;
	
	MovementSpline = UEmeraldBlueprintFunctionLibrary::CreateSplinePath(this, Start, End, PathPoints,GetOwner());
}

void UUnitMovementComponent::MoveAlongPath(float DeltaTime)
{
	if (!MovementSpline || !Unit) return;

	// Posun po spline
	SplineDistance += (bIsReversedMovement ? -UnitSpeed : UnitSpeed) * DeltaTime;
	SplineDistance = FMath::Clamp(SplineDistance, 0.0f, MovementSpline->GetSplineLength());

	FVector NewLocation = MovementSpline->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);
	FVector ForwardVector = MovementSpline->GetDirectionAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);

	// Add constant pitch
	FRotator TargetRotation = ForwardVector.Rotation();
	TargetRotation.Pitch += -2.0f; // Předklon

	//  (Roll)
	FVector FutureDirection = MovementSpline->GetDirectionAtDistanceAlongSpline(SplineDistance + 10.0f, ESplineCoordinateSpace::World);
	float Curvature = FVector::CrossProduct(ForwardVector, FutureDirection).Z;

	// Invert Roll
	float RollAmount = RollAmountMultiplier * FMath::Pow(FMath::Abs(Curvature), 1.5f) * FMath::Sign(Curvature);
	TargetRotation.Roll = RollAmount; 

	// Flying Unit Pitch Correction 
	if (Unit->bIsFlyingUnit)
	{
		 TargetRotation.Pitch += -8.0f;
	}

	// Rotation interpolation
	FRotator SmoothRotation = FMath::RInterpTo(GetOwner()->GetActorRotation(), TargetRotation, DeltaTime, SmoothRotationInterpSpeed);

	// New Rotation
	Unit->SetActorRotation(SmoothRotation);
	Unit->SetActorLocation(NewLocation);
}

void UUnitMovementComponent::Turn180()
{
	// TODO : Rotate Actor 180;
	bIsReversedMovement = !bIsReversedMovement;
}

void UUnitMovementComponent::ExtendMovementPathToReturn(FTransform Start, FTransform End)
{
	FVector CurrentLocation = Unit->GetActorLocation();
	FRotator CurrentRotation = Unit->GetActorRotation();

	float PitchDelta = FMath::RandRange(0.f, 20.f);
	float RandomDistance = FMath::RandRange(15.f, 50.f);
	
	CurrentRotation = FRotator(PitchDelta, CurrentRotation.Yaw, 0.f);

	/* Add first right turn */
	for (int i = 0; i < 3; i++)
	{
		FVector TurnPoint = GetTurnPoint(CurrentLocation, CurrentRotation, 45.f, RandomDistance);
		MovementSpline->AddSplinePoint(TurnPoint, ESplineCoordinateSpace::World);
		CurrentLocation = TurnPoint;
		CurrentRotation += FRotator(0.f, 45.f, 0.f);
	}

	/* Add point for straight */
	FVector LandingPoint = FVector(CurrentLocation.X, End.GetLocation().Y, CurrentLocation.Z) + FVector(0.f, 50.f, 0.f);
	MovementSpline->AddSplinePoint(LandingPoint, ESplineCoordinateSpace::World);

	CurrentLocation = LandingPoint;
	CurrentRotation = FRotator(-PitchDelta, CurrentRotation.Yaw, 0.f);

	/* Add second turn right */
	for (int i = 0; i < 3; i++)
	{
		FVector TurnPoint = GetTurnPoint(CurrentLocation, CurrentRotation, 45.f, RandomDistance);
		MovementSpline->AddSplinePoint(TurnPoint, ESplineCoordinateSpace::World);
		CurrentLocation = TurnPoint;
		CurrentRotation += FRotator(0.f, 45.f, 0.f);
	}

	/* Add Beyond Return point */
	FVector BeyondReturnPoint = End.GetLocation() - (End.Rotator().Vector() * 30.f); 
	MovementSpline->AddSplinePoint(BeyondReturnPoint, ESplineCoordinateSpace::World);

	/* Add Return point */
	MovementSpline->AddSplinePoint(End.GetLocation(), ESplineCoordinateSpace::World);
}



FVector UUnitMovementComponent::GetTurnPoint(FVector CurrentLocation, FRotator CurrentRotation, float TurnAngle,
	float Distance)
{
	FVector ForwardDirection = CurrentRotation.Vector();

	FRotator TurnRotation(0, TurnAngle, 0);
	FVector TurnDirection = TurnRotation.RotateVector(ForwardDirection);

	FVector TurnPoint = CurrentLocation + (TurnDirection * Distance);

	return TurnPoint;
}


void UUnitMovementComponent::StopUnit()
{
	UnitSpeed = 0.0f;
	bMovementEnabled = false;
}

void UUnitMovementComponent::Accelerate(float DeltaTime)
{
	// Restart Movement
	if (!Unit)
	{
		return;
	}
	
	bMovementEnabled = true;
	if (UnitSpeed < Unit->Speed)
	{
		UnitSpeed += DeltaTime * Acceleration;
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


