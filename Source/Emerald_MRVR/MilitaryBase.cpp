
#include "MilitaryBase.h"
#include "BoxComponent.h"
#include "DebugMacros.h"
#include "Components/BuildingsModuleComponent.h"
#include "Components/DownScaleComponent.h"
#include "CORE/MR_General.h"
#include "CORE/PC_MR_General.h"
#include "Data/BuildingDataAsset.h"
#include "Net/UnrealNetwork.h"

#define DRAW_SPHERE (Location) if (GetWorld()) DrawDebugSphere()


AMilitaryBase::AMilitaryBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	SetRootComponent(BaseBody);
	SetReplicates(true);
	
	BaseBox = CreateDefaultSubobject<UBoxComponent>("BaseBox");
	BaseBox->SetupAttachment(BaseBody);
	
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	
	SpawnPoint_Ground = CreateDefaultSubobject<USceneComponent>("SpawnPointGround");
	SpawnPoint_Ground->SetupAttachment(BaseBody);
	SpawnPoint_Air = CreateDefaultSubobject<USceneComponent>("SpawnPointAir");
	SpawnPoint_Air->SetupAttachment(BaseBody);

	// Modules
	Modules = CreateDefaultSubobject<USceneComponent>(TEXT("ModulesRoot"));
	Modules->SetupAttachment(BaseBody);
	SetReplicates(true);
}

void AMilitaryBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMilitaryBase, General);
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Ground)
	DOREPLIFETIME(AMilitaryBase, BaseBody)
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Air)
	DOREPLIFETIME(AMilitaryBase, ReplicatedBuildingComponents);
	DOREPLIFETIME(AMilitaryBase, BuildingModules);
	DOREPLIFETIME(AMilitaryBase, Modules);
	DOREPLIFETIME(AMilitaryBase, AvailableBuildings);
	DOREPLIFETIME(AMilitaryBase, ResourcesWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, HealthWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, OriginalMaterial);
	DOREPLIFETIME(AMilitaryBase, HoveredMaterial);
	
}

void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
	General = Cast<AMR_General>(GetOwner());
	DBG_ONE_PARAM(5, "general: %s", *General->GetName())
	
	if (ensure(General))
	{
		if (HasAuthority())
		{
			SpawnModules();
		}
		else
		{
			Server_SpawnModules();
		}
		{
			SpawnResourcesWidget();
			SpawnHealthWidget();
		}
		
		
		if (HealthWidgetInstance && ResourcesWidgetInstance)
		{
			General->bGameInitialized = true;
		}
	} 
}

void AMilitaryBase::SpawnResourcesWidget()
{
	if (!HasAuthority())
	{
		Server_SpawnResourcesWidget();
	}
	
	if (General && General->IsLocallyControlled())
	{
		TObjectPtr<UWorld> World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			FVector Location = GetActorLocation() + FVector(0.f, 0.f, 100.f);
			ResourcesWidgetInstance = World->SpawnActor<AActor>(ResourcesWBP, Location, FRotator::ZeroRotator, SpawnParams);
		}
	}
}

void AMilitaryBase::Server_SpawnResourcesWidget_Implementation()
{
	SpawnResourcesWidget();
}

void AMilitaryBase::SpawnHealthWidget()
{
	if (!HasAuthority())
	{
		Server_SpawnHealthWidget();
		return;
	}
	
	TObjectPtr<UWorld> World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FVector Location = GetActorLocation() + FVector(0.f, 0.f, 110.f);
		HealthWidgetInstance = World->SpawnActor<AActor>(HealthWidget, Location, FRotator::ZeroRotator, SpawnParams);
		if (HealthWidgetInstance)
		{
			HealthWidgetInstance->SetReplicates(true);
		}
	}
}

void AMilitaryBase::Server_SpawnHealthWidget_Implementation()
{
	SpawnHealthWidget();
}

void AMilitaryBase::SpawnModules()
{
	if (!HasAuthority())
	{
		Server_SpawnModules();
		return;
	}
	
	for (UBuildingDataAsset* Building : General->AvailableBuildings)
	{
		BuildingModules.AddUnique(Building);
		{
			UBuildingsModuleComponent* BuildingComp = NewObject<UBuildingsModuleComponent>(this, *Building->BuildingName.ToString());
			if (BuildingComp)
			{
				BuildingComp->SetIsReplicated(true);
				BuildingComp->SetupAttachment(Modules);
				BuildingComp->BuildingDataAsset = Building;
				BuildingComp->RegisterComponent();

				UStaticMeshComponent* ModuleMesh = NewObject<UStaticMeshComponent>(this, *Building->SM_Building->GetName());
				if (ModuleMesh)
				{
					ModuleMesh->SetIsReplicated(true);
					ModuleMesh->SetupAttachment(BuildingComp);
					ModuleMesh->SetStaticMesh(Building->SM_Building);
					ModuleMesh->SetMaterial(0, General->PlayerDefaultColor);
					BuildingComp->ModuleMeshInstance = ModuleMesh;
					ModuleMesh->RegisterComponent();
				}

				ReplicatedBuildingComponents.AddUnique(BuildingComp);
				BuildingComponentsMap.Add(Building->BuildingName, BuildingComp);
			}
		}
	}
}

void AMilitaryBase::Server_SpawnModules_Implementation()
{
	SpawnModules();
}



