#include "BuildingsModuleComponent.h"

#include "BoxComponent.h"


UBuildingsModuleComponent::UBuildingsModuleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UBuildingsModuleComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBuildingsModuleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

