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
	ModuleMesh->SetupAttachment(this);
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
	// ModuleMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
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

void UBuildingsModuleComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


void UBuildingsModuleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBuildingsModuleComponent::HighlightModule(bool bIsHighlighted)
{
	IBuildingsModuleInterface::HighlightModule(bIsHighlighted);

	DBG(0,"Interface")

	if (bIsHighlighted)
	{
		ModuleMesh->SetMaterial(0, MyBaseInstance->HoveredMaterial);
	}
	else
	{
		ModuleMesh->SetMaterial(0, MyBaseInstance->OriginalMaterial);
	}

}






