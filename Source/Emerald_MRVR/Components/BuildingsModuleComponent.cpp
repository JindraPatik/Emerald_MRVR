#include "BuildingsModuleComponent.h"
#include "Emerald_MRVR/Actors/MilitaryBase.h"
#include "Emerald_MRVR/CORE/VRPawn.h"
#include "Net/UnrealNetwork.h"


UBuildingsModuleComponent::UBuildingsModuleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UBuildingsModuleComponent::BeginPlay()
{
	Super::BeginPlay();
	AVRPawn* General = Cast<AVRPawn>(GetOwner()->GetOwner());
	MyBaseInstance = Cast<AMilitaryBase>(GetOwner());
}

void UBuildingsModuleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildingsModuleComponent, ModuleMeshInstance);
	DOREPLIFETIME(UBuildingsModuleComponent, BuildingDataAsset);
}

UBuildingsModuleComponent* UBuildingsModuleComponent::SelectModule()
{
	return this;
}













