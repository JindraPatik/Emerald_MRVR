#include "SpawnPointStation.h"

#include "ArrowComponent.h"
#include "BillboardComponent.h"

ASpawnPointStation::ASpawnPointStation()
{
	PrimaryActorTick.bCanEverTick = true;
	

	SpawnPointMine = CreateDefaultSubobject<USceneComponent>(TEXT("Mine"));
	SpawnPointMine->SetupAttachment(RootComponent);
	SpawnPointMine->ComponentTags.Add("Mine");
	SpawnPointComponents.Add(SpawnPointMine);
	
	SpawnPointRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SpawnPointRoot;
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);

	SpawnPointPrison = CreateDefaultSubobject<USceneComponent>(TEXT("Prison"));
	SpawnPointPrison->SetupAttachment(RootComponent);
	SpawnPointPrison->ComponentTags.Add("Prison");
	SpawnPointComponents.Add(SpawnPointPrison);

	SpawnPointVault = CreateDefaultSubobject<USceneComponent>(TEXT("Vault"));
	SpawnPointVault->SetupAttachment(RootComponent);
	SpawnPointVault->ComponentTags.Add("Vault");
	SpawnPointComponents.Add(SpawnPointVault);
	
	SpawnPointPit = CreateDefaultSubobject<USceneComponent>(TEXT("Pit"));
	SpawnPointPit->SetupAttachment(RootComponent);
	SpawnPointPit->ComponentTags.Add("Pit");
	SpawnPointComponents.Add(SpawnPointPit);

	SpawnPointSilo = CreateDefaultSubobject<USceneComponent>(TEXT("Silo"));
	SpawnPointSilo->SetupAttachment(RootComponent);
	SpawnPointSilo->ComponentTags.Add("Mine");
	SpawnPointComponents.Add(SpawnPointSilo);

	SpawnPointOutpost = CreateDefaultSubobject<USceneComponent>(TEXT("Outpost"));
	SpawnPointOutpost->SetupAttachment(RootComponent);
	SpawnPointOutpost->ComponentTags.Add("Outpost");
	SpawnPointComponents.Add(SpawnPointOutpost);

	SpawnPointFort = CreateDefaultSubobject<USceneComponent>(TEXT("Fort"));
	SpawnPointFort->SetupAttachment(RootComponent);
	SpawnPointFort->ComponentTags.Add("Fort");
	SpawnPointComponents.Add(SpawnPointFort);

	SpawnPointBastion = CreateDefaultSubobject<USceneComponent>(TEXT("Bastion"));
	SpawnPointBastion->SetupAttachment(RootComponent);
	SpawnPointBastion->ComponentTags.Add("Bastion");
	SpawnPointComponents.Add(SpawnPointBastion);

	SpawnPointPlatform = CreateDefaultSubobject<USceneComponent>(TEXT("Platform"));
	SpawnPointPlatform->SetupAttachment(RootComponent);
	SpawnPointPlatform->ComponentTags.Add("Platform");
	SpawnPointComponents.Add(SpawnPointPlatform);

	SpawnPointDepot = CreateDefaultSubobject<USceneComponent>(TEXT("Depot"));
	SpawnPointDepot->SetupAttachment(RootComponent);
	SpawnPointDepot->ComponentTags.Add("Depot");
	SpawnPointComponents.Add(SpawnPointDepot);

	SpawnPointHelipad = CreateDefaultSubobject<USceneComponent>(TEXT("Helipad"));
	SpawnPointHelipad->SetupAttachment(RootComponent);
	SpawnPointHelipad->ComponentTags.Add("Helipad");
	SpawnPointComponents.Add(SpawnPointHelipad);

	SpawnPointRunway = CreateDefaultSubobject<USceneComponent>(TEXT("Runway"));
	SpawnPointRunway->SetupAttachment(RootComponent);
	SpawnPointRunway->ComponentTags.Add("Runway");
	SpawnPointComponents.Add(SpawnPointRunway);

	SpawnPointAerial = CreateDefaultSubobject<USceneComponent>(TEXT("Aerial"));
	SpawnPointAerial->SetupAttachment(RootComponent);
	SpawnPointAerial->ComponentTags.Add("Aerial");
	SpawnPointComponents.Add(SpawnPointAerial);

	SpawnPointHangar = CreateDefaultSubobject<USceneComponent>(TEXT("Hangar"));
	SpawnPointHangar->SetupAttachment(RootComponent);
	SpawnPointHangar->ComponentTags.Add("Hangar");
	SpawnPointComponents.Add(SpawnPointHangar);


	if (SpawnPointComponents.Num() > 0)
	{
		for (USceneComponent* SpawnPoint : SpawnPointComponents)
		{
			FString PreviewMeshName = FString::Printf(TEXT("PreviewMesh_%s"), *SpawnPoint->GetName());
			PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(*PreviewMeshName);
			PreviewMesh->SetupAttachment(SpawnPoint);
		}
	}
}

void ASpawnPointStation::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(true);
}

