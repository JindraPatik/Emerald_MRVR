#include "SpawnPointStation.h"

#include "BillboardComponent.h"

ASpawnPointStation::ASpawnPointStation()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorHiddenInGame(true);

	SpawnPointRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SpawnPointRoot;
	BillboardComponent->CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SpawnPointMine = CreateDefaultSubobject<USceneComponent>(TEXT("Mine"));
	SpawnPointMine->SetupAttachment(RootComponent);
	SpawnPointMine->ComponentTags.Add("Mine");
	SpawnPoints.Add(SpawnPointMine);

	SpawnPointPrison = CreateDefaultSubobject<USceneComponent>(TEXT("Prison"));
	SpawnPointPrison->SetupAttachment(RootComponent);
	SpawnPointPrison->ComponentTags.Add("Prison");
	SpawnPoints.Add(SpawnPointPrison);

	SpawnPointVault = CreateDefaultSubobject<USceneComponent>(TEXT("Vault"));
	SpawnPointVault->SetupAttachment(RootComponent);
	SpawnPointVault->ComponentTags.Add("Vault");
	SpawnPoints.Add(SpawnPointVault);
	
	SpawnPointPit = CreateDefaultSubobject<USceneComponent>(TEXT("Pit"));
	SpawnPointPit->SetupAttachment(RootComponent);
	SpawnPointPit->ComponentTags.Add("Pit");
	SpawnPoints.Add(SpawnPointPit);

	SpawnPointSilo = CreateDefaultSubobject<USceneComponent>(TEXT("Silo"));
	SpawnPointSilo->SetupAttachment(RootComponent);
	SpawnPointSilo->ComponentTags.Add("Mine");
	SpawnPoints.Add(SpawnPointSilo);

	SpawnPointOutpost = CreateDefaultSubobject<USceneComponent>(TEXT("Outpost"));
	SpawnPointOutpost->SetupAttachment(RootComponent);
	SpawnPointOutpost->ComponentTags.Add("Outpost");
	SpawnPoints.Add(SpawnPointOutpost);

	SpawnPointFort = CreateDefaultSubobject<USceneComponent>(TEXT("Fort"));
	SpawnPointFort->SetupAttachment(RootComponent);
	SpawnPointFort->ComponentTags.Add("Fort");
	SpawnPoints.Add(SpawnPointFort);

	SpawnPointBastion = CreateDefaultSubobject<USceneComponent>(TEXT("Bastion"));
	SpawnPointBastion->SetupAttachment(RootComponent);
	SpawnPointBastion->ComponentTags.Add("Bastion");
	SpawnPoints.Add(SpawnPointBastion);

	SpawnPointPlatform = CreateDefaultSubobject<USceneComponent>(TEXT("Platform"));
	SpawnPointPlatform->SetupAttachment(RootComponent);
	SpawnPointPlatform->ComponentTags.Add("Platform");
	SpawnPoints.Add(SpawnPointPlatform);

	SpawnPointDepot = CreateDefaultSubobject<USceneComponent>(TEXT("Depot"));
	SpawnPointDepot->SetupAttachment(RootComponent);
	SpawnPointDepot->ComponentTags.Add("Depot");
	SpawnPoints.Add(SpawnPointDepot);

	SpawnPointHelipad = CreateDefaultSubobject<USceneComponent>(TEXT("Helipad"));
	SpawnPointHelipad->SetupAttachment(RootComponent);
	SpawnPointHelipad->ComponentTags.Add("Helipad");
	SpawnPoints.Add(SpawnPointHelipad);

	SpawnPointRunway = CreateDefaultSubobject<USceneComponent>(TEXT("Runway"));
	SpawnPointRunway->SetupAttachment(RootComponent);
	SpawnPointRunway->ComponentTags.Add("Runway");
	SpawnPoints.Add(SpawnPointRunway);

	SpawnPointAerial = CreateDefaultSubobject<USceneComponent>(TEXT("Aerial"));
	SpawnPointAerial->SetupAttachment(RootComponent);
	SpawnPointAerial->ComponentTags.Add("Aerial");
	SpawnPoints.Add(SpawnPointAerial);

	SpawnPointHangar = CreateDefaultSubobject<USceneComponent>(TEXT("Hangar"));
	SpawnPointHangar->SetupAttachment(RootComponent);
	SpawnPointHangar->ComponentTags.Add("Hangar");
	SpawnPoints.Add(SpawnPointHangar);


	if (SpawnPoints.Num() > 0)
	{
		for (USceneComponent* SpawnPoint : SpawnPoints)
		{
			FString BillboardName = FString::Printf(TEXT("BillboardComponent_%s"), *SpawnPoint->GetName());
			BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(*BillboardName);
			BillboardComponent->SetupAttachment(SpawnPoint);
		}
	}
}

void ASpawnPointStation::BeginPlay()
{
	Super::BeginPlay();
	
}

