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

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	ModuleMeshRoot = CreateDefaultSubobject<UStaticMeshComponent>("ModuleBody");
	ModuleMeshRoot->SetupAttachment(SceneRoot);
}

void AModuleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AModuleActor, ModuleMeshRoot);
	DOREPLIFETIME(AModuleActor, BuildingDataAsset);
	DOREPLIFETIME(AModuleActor, OriginalMaterial);
	DOREPLIFETIME(AModuleActor, HoverMaterial);
	DOREPLIFETIME(AModuleActor, SceneRoot);
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
	}
	else
	{
		DBG(5, "NOT BuildingDataAsset")
	}
}
