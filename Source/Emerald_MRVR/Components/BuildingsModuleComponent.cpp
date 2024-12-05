#include "BuildingsModuleComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/Interfaces/BuildingsModuleInterface.h"
#include "Net/UnrealNetwork.h"


UBuildingsModuleComponent::UBuildingsModuleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UBuildingsModuleComponent::BeginPlay()
{
	Super::BeginPlay();
	AMR_General* General = Cast<AMR_General>(GetOwner()->GetOwner());
	MyBaseInstance = Cast<AMilitaryBase>(GetOwner());

	if (ModuleMeshInstance)
	{
		ModuleMeshInstance->SetMaterial(0, General->PlayerDefaultColor);
	}
	else
	{
		DBG(5, "INSTANCE not valid")
	}

}

void UBuildingsModuleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBuildingsModuleComponent, ModuleMeshInstance);
	DOREPLIFETIME(UBuildingsModuleComponent, BuildingDataAsset);
}

void UBuildingsModuleComponent::InitializeComponent()
{
	Super::InitializeComponent();
	// RegisterComponent();
}


void UBuildingsModuleComponent::HighlightModule(bool bIsHighlighted)
{
	IBuildingsModuleInterface::HighlightModule(bIsHighlighted);

	DBG(0,"Interface")

	if (bIsHighlighted)
	{
		ModuleMeshInstance->SetMaterial(0, MyBaseInstance->HoveredMaterial);
	}
	else
	{
		ModuleMeshInstance->SetMaterial(0, MyBaseInstance->OriginalMaterial);
	}

}






