#include "BuildingsModuleComponent.h"

#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Net/UnrealNetwork.h"


UBuildingsModuleComponent::UBuildingsModuleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	ModuleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ModuleMesh"));
	ModuleMesh->SetIsReplicated(true); 
	
}


void UBuildingsModuleComponent::BeginPlay()
{
	Super::BeginPlay();
	AMR_General* General = Cast<AMR_General>(GetOwner()->GetOwner());
	if (BuildingDataAsset)
	{
		ModuleMesh->SetStaticMesh(BuildingDataAsset->SM_Building);
		if (General)
		{
			ModuleMesh->SetMaterial(0, General->PlayerDefaultColor);
		}
	}
	ModuleMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	ModuleMesh->RegisterComponent();

}

void UBuildingsModuleComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

void UBuildingsModuleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildingsModuleComponent, ModuleMesh);
	DOREPLIFETIME(UBuildingsModuleComponent, BuildingDataAsset);
}


void UBuildingsModuleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

