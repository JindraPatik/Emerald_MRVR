
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
	BuildingsModuleComponent->SetIsReplicated(true);
	
	SpawnPoint_Ground = CreateDefaultSubobject<USceneComponent>("SpawnPointGround");
	SpawnPoint_Ground->SetupAttachment(RootComponent);
	SpawnPoint_Air = CreateDefaultSubobject<USceneComponent>("SpawnPointAir");
	SpawnPoint_Air->SetupAttachment(RootComponent);

	// Modules
	Modules = CreateDefaultSubobject<USceneComponent>(TEXT("ModulesRoot"));
	Modules->SetupAttachment(RootComponent);
	Modules->SetIsReplicated(true);
	Modules->RegisterComponent();
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
	DOREPLIFETIME(AMilitaryBase, ResourcesWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, HealthWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, OriginalMaterial);
	DOREPLIFETIME(AMilitaryBase, HoveredMaterial);
}



void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
	AMR_General* General = Cast<AMR_General>(GetOwner());
	
	if (ensure(General))
	{
		SpawnModules();
		SpawnResourcesWidget();
		SpawnHealthWidget();
		
		if (HealthWidgetInstance && ResourcesWidgetInstance)
		{
			General->bGameInitialized = true;
		}
	} 
}

void AMilitaryBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void AMilitaryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMilitaryBase::SpawnResourcesWidget()
{
	if (!HasAuthority())
	{
		Server_SpawnResourcesWidget();
	}
	
	AMR_General* General = Cast<AMR_General>(GetOwner());

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
	AMR_General* General = Cast<AMR_General>(GetOwner());
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
	AMR_General* General = Cast<AMR_General>(GetOwner());
	if (!HasAuthority())
	{
		Server_SpawnModules();
	}
	for (UBuildingDataAsset* Building : General->AvailableBuildings)
	{
		BuildingModules.AddUnique(Building);
		if (General->IsLocallyControlled())
		{
			UBuildingsModuleComponent* BuildingComp = NewObject<UBuildingsModuleComponent>(this, *Building->BuildingName.ToString());
			
			if (BuildingComp)
			{
				BuildingComp->SetIsReplicated(true);
				Modules->RegisterComponent();
				BuildingComp->SetupAttachment(Modules); // Modules musí být registrované
				BuildingComp->BuildingDataAsset = Building;
				BuildingComp->RegisterComponent();

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



