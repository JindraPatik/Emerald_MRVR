#include "ModuleActor.h"
#include "DebugMacros.h"
#include "Components/DownScaleComponent.h"
#include "CORE/MR_General.h"
#include "Data/BuildingDataAsset.h"
#include "Net/UnrealNetwork.h"

AModuleActor::AModuleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	
	ModuleMesh = CreateDefaultSubobject<UStaticMeshComponent>("ModuleBody");
	SetRootComponent(ModuleMesh);
}

void AModuleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AModuleActor, ModuleMesh);
	DOREPLIFETIME(AModuleActor, BuildingDataAsset);
	DOREPLIFETIME(AModuleActor, OriginalMaterial);
	DOREPLIFETIME(AModuleActor, HoverMaterial);
}

void AModuleActor::BeginPlay()
{
	Super::BeginPlay();
	General = General ? General : Cast<AMR_General>(GetOwner());
}

void AModuleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AModuleActor::OnBuildingsDataChanged()
{
	General = General ? General : Cast<AMR_General>(GetOwner());
	if (BuildingDataAsset)
	{
		ModuleMesh->SetStaticMesh(BuildingDataAsset->SM_Building);
		if (General)
		{
			ModuleMesh->SetMaterial(0, General->PlayerDefaultColor);
			OriginalMaterial = General->PlayerDefaultColor;
		}
	}
	else
	{
		DBG(5, "NOT BuildingDataAsset")
	}
}

void AModuleActor::HighlightModule(bool bIsHighlighted)
{
	IBuildingsModuleInterface::HighlightModule(bIsHighlighted);

	bIsHighlighted ? ModuleMesh->SetMaterial(0, HoverMaterial) : ModuleMesh->SetMaterial(0, OriginalMaterial);
}

