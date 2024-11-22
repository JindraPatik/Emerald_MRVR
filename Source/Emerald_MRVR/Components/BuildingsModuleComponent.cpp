#include "BuildingsModuleComponent.h"

#include "BoxComponent.h"
#include "Emerald_MRVR/CORE/MR_General.h"


UBuildingsModuleComponent::UBuildingsModuleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	SetIsReplicated(true);
}


void UBuildingsModuleComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBuildingsModuleComponent::PostInitProperties()
{
	Super::PostInitProperties();
}


void UBuildingsModuleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

