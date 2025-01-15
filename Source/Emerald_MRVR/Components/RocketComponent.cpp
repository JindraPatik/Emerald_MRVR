#include "RocketComponent.h"

#include "EngineUtils.h"
#include "Emerald_MRVR/Unit.h"

URocketComponent::URocketComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URocketComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentSpeed = 0.0f; // Startovní rychlost
	CurrentDirection = GetOwner()->GetActorForwardVector().GetSafeNormal(); // Výchozí směr letu
	SearchTargets();

	GetWorld()->GetTimerManager().SetTimer(LaunchHandle, this, &URocketComponent::RocketStarted, LaunchTime, false);
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
	CurrentSpeed += DeltaTime; 
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

	// Pohyb ve směru `CurrentDirection`
	FVector DeltaLocation = CurrentDirection * CurrentSpeed * DeltaTime;
	Owner->AddActorWorldOffset(DeltaLocation); // true = s kolizí

	// Nastavení rotace ve směru letu
	FRotator TargetRotation = CurrentDirection.Rotation();
	Owner->SetActorRotation(TargetRotation);
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

