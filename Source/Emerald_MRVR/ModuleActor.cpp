#include "ModuleActor.h"

#include "DebugMacros.h"
#include "Components/DownScaleComponent.h"
#include "Components/MilitaryBaseComp.h"
#include "CORE/MR_General.h"
#include "Data/BuildingDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Emerald_MRVR/CORE/MR_General.h"

AModuleActor::AModuleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	
	ModuleBody = CreateDefaultSubobject<UStaticMeshComponent>("ModuleBody");
	SetRootComponent(ModuleBody);
}

void AModuleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AModuleActor, ModuleBody);
	DOREPLIFETIME(AModuleActor, BuildingDataAsset);
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
		ModuleBody->SetStaticMesh(BuildingDataAsset->SM_Building);
		if (General)
		{
			ModuleBody->SetMaterial(0, General->PlayerDefaultColor);
		}
	}
	else
	{
		DBG(5, "NOT BuildingDataAsset")
	}
}



