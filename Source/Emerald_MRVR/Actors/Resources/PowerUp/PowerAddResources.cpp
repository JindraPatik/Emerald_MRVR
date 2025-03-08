#include "PowerAddResources.h"

#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"


APowerAddResources::APowerAddResources(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APowerAddResources::BeginPlay()
{
	Super::BeginPlay();
	
}

void APowerAddResources::Activate()
{
	Super::Activate();

	UResourcesComponent* ResourcesComponent = PowerUpOwner->FindComponentByClass<UResourcesComponent>();
	if (!ResourcesComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PowerAddResources: %s() Not Found!"), *ResourcesComponent->GetName());
		return;
	}
	ResourcesComponent->AvailableResources += ResourcesToAdd;
}


