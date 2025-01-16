#include "RocketComponent.h"

#include "BoxComponent.h"
#include "EngineUtils.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/Unit.h"

URocketComponent::URocketComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URocketComponent::BeginPlay()
{
	Super::BeginPlay();
	AUnit* Unit = Cast<AUnit>(GetOwner());
	if (Unit)
	{
		Unit->BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &URocketComponent::OnBoxOverlapped);
	}
	CurrentSpeed = 0.0f; // Startovní rychlost
	CurrentDirection = GetOwner()->GetActorForwardVector().GetSafeNormal(); // Výchozí směr letu
	SearchTargets();
	GetWorld()->GetTimerManager().SetTimer(LaunchHandle, this, &URocketComponent::RocketStarted, LaunchTime, false);
	Missed();
}

void URocketComponent::OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		KillMe();
	}
}

void URocketComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()) return;

	!bRocketHasLaunched ? LaunchRocket(DeltaTime) : MoveToTarget(DeltaTime);
}

void URocketComponent::FindTarget()
{
	UWorld* World = GetWorld();
	if (!World) return;
	ListOfTargets.Empty();

	for (TActorIterator<AUnit> It(World); It; ++It)
	{
		AUnit* Unit = *It;
		if (Unit)
		{
			if (Unit->GetOwner() != GetOwner()->GetOwner() && Unit->bIsFlyingUnit)
			{
				ListOfTargets.Add(Unit);
			}
		}
	}
}

void URocketComponent::SelectTarget()
{
	if (ListOfTargets.Num() > 0)
	{
		int ItemsInArray = ListOfTargets.Num() - 1;
		int Index = FMath::RandRange(0, ItemsInArray);
		Target = ListOfTargets[Index];
	}
}

void URocketComponent::LaunchRocket(float DeltaTime)
{
	AActor* Owner = GetOwner();
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, MaxLaunchSpeed, DeltaTime, StartAccelerationRate);
	FVector DeltaLocation = CurrentDirection * CurrentSpeed * DeltaTime;
	Owner->AddActorWorldOffset(DeltaLocation);
}

void URocketComponent::RocketStarted()
{
	bRocketHasLaunched = true;
}

void URocketComponent::MoveToTarget(float DeltaTime)
{
	AActor* Owner = GetOwner();

	// Zrychlení
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, MaxSpeed, DeltaTime, AccelerationRate);

	// Pokud je Target platný, nastav směr k němu
	if (Target)
	{
		FVector TargetLocation = Target->GetActorLocation();
		FVector CurrentLocation = Owner->GetActorLocation();

		// Aktualizace směru k cíli
		CurrentDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
	}

	// Pohyb ve směru `CurrentDirection
	FVector DeltaLocation = CurrentDirection * CurrentSpeed * DeltaTime;
	Owner->AddActorWorldOffset(DeltaLocation); // Posun s kolizí (true)

	// Plynulá rotace směrem k cíli
	FRotator CurrentRotation = Owner->GetActorRotation();
	FRotator TargetRotation = CurrentDirection.Rotation();
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

	Owner->SetActorRotation(SmoothRotation);
}

void URocketComponent::SearchTargets()
{
	if (!Target)
	{
		GetWorld()->GetTimerManager().SetTimer(SearchHandle, this, &URocketComponent::FindAndSelect, 0.5f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SearchHandle);
	}
}

void URocketComponent::FindAndSelect()
{
	FindTarget();
	SelectTarget();
}

void URocketComponent::KillMe()
{
	DBG(5, "KILLED")
	GetOwner()->Destroy();
}

void URocketComponent::Missed()
{
	GetWorld()->GetTimerManager().SetTimer(MissedHandle, this, &URocketComponent::KillMe, Lifetime, false);
}

