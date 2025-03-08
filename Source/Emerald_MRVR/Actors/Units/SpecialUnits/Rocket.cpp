#include "Rocket.h"

#include "EngineUtils.h"

ARocket::ARocket(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ARocket::OnOverlapped);
	
	CurrentSpeed = 0.0f; // Starting speed
	CurrentDirection = GetActorForwardVector().GetSafeNormal(); // Default direction
	
	SearchTargets();
	GetWorld()->GetTimerManager().SetTimer(LaunchHandle, this, &ARocket::RocketStarted, LaunchTime, false);
	Missed();
}

void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	!bRocketHasLaunched ? LaunchRocket(DeltaTime) : MoveToTarget(DeltaTime);

	if (bHasTargeted && !Target)
	{
		PreviousTarget = nullptr;
		KillMe();
	}
	
}

void ARocket::OnOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		KillTarget(OtherActor);
		KillMe();
	}
}


void ARocket::FindTarget()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
		
	ListOfTargets.Empty();

	for (TActorIterator<AUnit> It(World); It; ++It)
	{
		AUnit* Unit = *It;
		if (!Unit)
		{
			return;
		}
		
		if (Unit->GetOwner() != GetOwner() && Unit->bIsFlyingUnit)
		{
			ListOfTargets.Add(Unit);
		}
	}
}

void ARocket::SelectTarget()
{
	if (ListOfTargets.Num() > 0)
	{
		int ItemsInArray = ListOfTargets.Num() - 1;
		int Index = FMath::RandRange(0, ItemsInArray);
		Target = ListOfTargets[Index];
		if (Target)
		{
			bHasTargeted = true;
		}
	}
}

void ARocket::LaunchRocket(float DeltaTime)
{
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, MaxLaunchSpeed, DeltaTime, StartAccelerationRate);
	FVector DeltaLocation = CurrentDirection * CurrentSpeed * DeltaTime;
	AddActorWorldOffset(DeltaLocation);
}

void ARocket::RocketStarted()
{
	bRocketHasLaunched = true;
}

void ARocket::MoveToTarget(float DeltaTime)
{
	/* Acceleration */
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, MaxSpeed, DeltaTime, AccelerationRate);

	/* Initial start upwards */
	if (Target && !bLaunched)
	{
		FVector TargetLocation = Target->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();

		FVector LaunchDirection = FVector(0, 0, 1);
		CurrentVelocity = LaunchDirection * CurrentSpeed * DeltaTime;

		bLaunched = true; // Rocket has been launched
	}

	// If target is valid, move towards
	if (Target)
	{
		FVector TargetLocation = Target->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();

		FVector TargetDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
		CurrentDirection = FMath::VInterpTo(CurrentDirection, TargetDirection, DeltaTime, DirectionChangeSpeed);
	}

	// Přidání gravitace do aktuální rychlosti
	CurrentVelocity.Z -= Gravity * DeltaTime;

	// Pohyb rakety na základě aktuální rychlosti a směru
	FVector DeltaLocation = CurrentDirection * CurrentSpeed * DeltaTime;
	AddActorWorldOffset(DeltaLocation, true); // Pohyb s kolizí

	// Plynulá rotace ve směru pohybu
	if (!CurrentVelocity.IsNearlyZero())
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = CurrentDirection.Rotation();
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
		SetActorRotation(SmoothRotation);
	}
}

void ARocket::SearchTargets()
{
	if (!Target)
	{
		GetWorld()->GetTimerManager().SetTimer(SearchHandle, this, &ARocket::FindAndSelectTarget, 0.5f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SearchHandle);
	}
}

void ARocket::FindAndSelectTarget()
{
	FindTarget();
	SelectTarget();
}

void ARocket::DestroyMe()
{
	/* TODO: Override KillMe with particles etc. */
	KillMe();
}

void ARocket::Missed()
{
	GetWorld()->GetTimerManager().SetTimer(MissedHandle, this, &ARocket::KillMe, Lifetime, false);
}

void ARocket::KillTarget(AActor* TargetActor)
{
	if (TargetActor)
	{
		AUnit* TargetEnemy = Cast<AUnit>(TargetActor);
		if (TargetEnemy && TargetEnemy->GetOwner() && TargetEnemy->GetOwner() != GetOwner())
		{
			TargetEnemy->Destroy();
		}
	}
}


