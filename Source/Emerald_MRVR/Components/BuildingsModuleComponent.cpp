#include "BuildingsModuleComponent.h"

#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Interfaces/BuildingsModuleInterface.h"
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
	MyBaseInstance = Cast<AMilitaryBase>(GetOwner());
	
	if (BuildingDataAsset)
	{
		ModuleMesh->SetStaticMesh(BuildingDataAsset->SM_Building);
		if (General)
		{
			ModuleMesh->SetMaterial(0, General->PlayerDefaultColor);
			if (MyBaseInstance)
			{
				MyBaseInstance->OriginalMaterial = General->PlayerDefaultColor;
			}
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

void UBuildingsModuleComponent::HighlightModule(bool bIsHighlighted)
{
	IBuildingsModuleInterface::HighlightModule(bIsHighlighted);

	UMaterialInterface* NewMaterial = bIsHighlighted ? MyBaseInstance->HoveredMaterial : MyBaseInstance->OriginalMaterial;


		if (GetOwner()->HasAuthority())
		{
			if (MyBaseInstance && ModuleMesh)
			{
				Multi_HighlightModule(NewMaterial);
				// DBG(1.f, "Change Material");
			}
		}
		else
		{
			if (MyBaseInstance && ModuleMesh)
			{
				Server_HighlightModule(NewMaterial);
				// DBG(1.f, "Change Material");
			}
		}

}

void UBuildingsModuleComponent::Server_HighlightModule_Implementation(UMaterialInterface* Material)
{
	Multi_HighlightModule(Material);
}

void UBuildingsModuleComponent::Multi_HighlightModule_Implementation(UMaterialInterface* Material)
{
	if (ModuleMesh)
	{
		ModuleMesh->SetMaterial(0, Material);
		ModuleMesh->MarkRenderStateDirty();
		
	}
}




