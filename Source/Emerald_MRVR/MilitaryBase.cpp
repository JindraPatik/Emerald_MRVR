
#include "MilitaryBase.h"
#include "BoxComponent.h"
#include "DebugMacros.h"
#include "Components/BuildingsModuleComponent.h"
#include "Components/DownScaleComponent.h"
#include "CORE/MR_General.h"
#include "Data/BuildingDataAsset.h"
#include "Net/UnrealNetwork.h"

#define DRAW_SPHERE (Location) if (GetWorld()) DrawDebugSphere()


AMilitaryBase::AMilitaryBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	RootComponent = BaseBody;
	
	BaseBox = CreateDefaultSubobject<UBoxComponent>("BaseBox");
	BaseBox->SetupAttachment(BaseBody);
	
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	BuildingsModuleComponent = CreateDefaultSubobject<UBuildingsModuleComponent>("BuildingsModuleComponent");
	
	SpawnPoint_Ground = CreateDefaultSubobject<USceneComponent>("SpawnPointGround");
	SpawnPoint_Ground->SetupAttachment(RootComponent);
	SpawnPoint_Air = CreateDefaultSubobject<USceneComponent>("SpawnPointAir");
	SpawnPoint_Air->SetupAttachment(RootComponent);

	// Modules
	Modules = CreateDefaultSubobject<USceneComponent>(TEXT("ModulesRoot"));
	Modules->SetupAttachment(RootComponent);

	// Postupne doplnit vsechny moduly!!!

	
}

void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMilitaryBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AMR_General* General = Cast<AMR_General>(GetOwner());
	
	if (ensure(General))
	{
		for (UBuildingDataAsset* Building : General->AvailableBuildings)
		{
			BuildingModules.AddUnique(Building);
			UBuildingsModuleComponent* BuildingComp = NewObject<UBuildingsModuleComponent>(this, *Building->BuildingName.ToString());
			if (BuildingComp)
			{
				BuildingComp->SetIsReplicated(true);
				BuildingComp->SetupAttachment(Modules);
				BuildingComp->BuildingDataAsset = Building;
				// BuildingComp->ModuleBody->SetStaticMesh(Building->SM_Building);
				
				ReplicatedBuildingComponents.AddUnique(BuildingComp);
				BuildingComponentsMap.Add(Building->BuildingName, BuildingComp);
			}
		General->bGameInitialized = true;
		}
	} 
}

void AMilitaryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMilitaryBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Ground)
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Air)
	DOREPLIFETIME(AMilitaryBase, ReplicatedBuildingComponents);
	DOREPLIFETIME(AMilitaryBase, BuildingsModuleComponent);
	DOREPLIFETIME(AMilitaryBase, BuildingModules);
	DOREPLIFETIME(AMilitaryBase, Modules);
	DOREPLIFETIME(AMilitaryBase, AvailableBuildings);
}




