#include "BuildingsComponent.h"
#include "Emerald_MRVR/Actors/MilitaryBase.h"
#include "Emerald_MRVR/CORE/VRPawn.h"
#include "Net/UnrealNetwork.h"


UBuildingsComponent::UBuildingsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UBuildingsComponent::BeginPlay()
{
	Super::BeginPlay();
	AVRPawn* General = Cast<AVRPawn>(GetOwner()->GetOwner());
	MyBaseInstance = Cast<AMilitaryBase>(GetOwner());
}

void UBuildingsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildingsComponent, ModuleMeshInstance);
	DOREPLIFETIME(UBuildingsComponent, BuildingDataAsset);
}

UBuildingsComponent* UBuildingsComponent::SelectModule()
{
	return this;
}













