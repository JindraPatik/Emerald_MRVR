#include "ResourcesComponent.h"


UResourcesComponent::UResourcesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	AvailableResources = 100.f;
}


void UResourcesComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UResourcesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

