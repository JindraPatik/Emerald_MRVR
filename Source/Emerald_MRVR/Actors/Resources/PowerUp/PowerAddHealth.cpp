#include "PowerAddHealth.h"

#include "Emerald_MRVR/Components/Health/HealthComponent.h"

APowerAddHealth::APowerAddHealth()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APowerAddHealth::BeginPlay()
{
	Super::BeginPlay();
	
}

void APowerAddHealth::Activate()
{
	Super::Activate();

	UHealthComponent* HealthComponent = PowerUpOwner->FindComponentByClass<UHealthComponent>();
	if (!HealthComponent)
	{
		return;
	}

	HealthComponent->Health += HealthToAdd;
	UE_LOG(LogTemp, Warning, TEXT("PowerAddHealth: Added %f to HEALTH"), HealthToAdd);
}
