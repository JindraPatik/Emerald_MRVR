#include "BuildingsModuleComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Net/UnrealNetwork.h"

UBuildingsModuleComponent::UBuildingsModuleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	SetIsReplicated(true);
	
	ModuleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ModuleMesh"));
	ModuleMesh->SetIsReplicated(true);
}

void UBuildingsModuleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildingsModuleComponent, ModuleMesh);
	DOREPLIFETIME(UBuildingsModuleComponent, BuildingDataAsset);
	DOREPLIFETIME(UBuildingsModuleComponent, BaseMaterial);
	DOREPLIFETIME(UBuildingsModuleComponent, HoverMaterial);
}

void UBuildingsModuleComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (BuildingDataAsset)
	{
		ModuleMesh->SetStaticMesh(BuildingDataAsset->SM_Building); // Příklad nastavení statického meshe podle Data Assetu
		BaseMaterial = BuildingDataAsset->BaseMaterial; 
	}
	ModuleMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	ModuleMesh->RegisterComponent();
}

void UBuildingsModuleComponent::PostInitProperties()
{
	Super::PostInitProperties();
}



void UBuildingsModuleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBuildingsModuleComponent::OnRep_SetInitialMaterial()
{
	if (BuildingDataAsset->BaseMaterial)
	{
		ModuleMesh->SetMaterial(0, BuildingDataAsset->BaseMaterial);
	}
}

void UBuildingsModuleComponent::OnRep_OnHoverMaterialChanged()
{
	/*if (BuildingDataAsset->HoverMaterial)
	{
		ModuleMesh->SetMaterial(0, BuildingDataAsset->HoverMaterial);
	}*/
}

void UBuildingsModuleComponent::OnModuleHovered()
{
	ModuleMesh->SetMaterial(0, BuildingDataAsset->HoverMaterial);
}

void UBuildingsModuleComponent::OnModuleUnHovered()
{
	ModuleMesh->SetMaterial(0, BuildingDataAsset->BaseMaterial);
}




