#include "SpawnPointStation.h"

#include "ArrowComponent.h"
#include "BillboardComponent.h"

ASpawnPointStation::ASpawnPointStation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpawnPointRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Root"));
	RootComponent = SpawnPointRoot;

	ArrowComponent = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this,TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->ArrowSize = 0.1f;
	ArrowComponent->ArrowColor = FColor::Yellow;
	
	SpawnPointMine = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Mine"));
	SpawnPointMine->SetupAttachment(RootComponent);
	SpawnPointMine->ComponentTags.Add("Mine");
	SpawnPointComponents.Add(SpawnPointMine);

	SpawnPointPrison = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Prison"));
	SpawnPointPrison->SetupAttachment(RootComponent);
	SpawnPointPrison->ComponentTags.Add("Prison");
	SpawnPointComponents.Add(SpawnPointPrison);

	SpawnPointVault = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Vault"));
	SpawnPointVault->SetupAttachment(RootComponent);
	SpawnPointVault->ComponentTags.Add("Vault");
	SpawnPointComponents.Add(SpawnPointVault);
	
	SpawnPointPit = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Pit"));
	SpawnPointPit->SetupAttachment(RootComponent);
	SpawnPointPit->ComponentTags.Add("Pit");
	SpawnPointComponents.Add(SpawnPointPit);

	SpawnPointSilo = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Silo"));
	SpawnPointSilo->SetupAttachment(RootComponent);
	SpawnPointSilo->ComponentTags.Add("Mine");
	SpawnPointComponents.Add(SpawnPointSilo);

	SpawnPointOutpost = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Outpost"));
	SpawnPointOutpost->SetupAttachment(RootComponent);
	SpawnPointOutpost->ComponentTags.Add("Outpost");
	SpawnPointComponents.Add(SpawnPointOutpost);

	SpawnPointFort = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Fort"));
	SpawnPointFort->SetupAttachment(RootComponent);
	SpawnPointFort->ComponentTags.Add("Fort");
	SpawnPointComponents.Add(SpawnPointFort);

	SpawnPointBastion = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Bastion"));
	SpawnPointBastion->SetupAttachment(RootComponent);
	SpawnPointBastion->ComponentTags.Add("Bastion");
	SpawnPointComponents.Add(SpawnPointBastion);

	SpawnPointPlatform = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Platform"));
	SpawnPointPlatform->SetupAttachment(RootComponent);
	SpawnPointPlatform->ComponentTags.Add("Platform");
	SpawnPointComponents.Add(SpawnPointPlatform);

	SpawnPointDepot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Depot"));
	SpawnPointDepot->SetupAttachment(RootComponent);
	SpawnPointDepot->ComponentTags.Add("Depot");
	SpawnPointComponents.Add(SpawnPointDepot);

	SpawnPointHelipad = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Helipad"));
	SpawnPointHelipad->SetupAttachment(RootComponent);
	SpawnPointHelipad->ComponentTags.Add("Helipad");
	SpawnPointComponents.Add(SpawnPointHelipad);

	SpawnPointRunway = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Runway"));
	SpawnPointRunway->SetupAttachment(RootComponent);
	SpawnPointRunway->ComponentTags.Add("Runway");
	SpawnPointComponents.Add(SpawnPointRunway);

	SpawnPointAerial = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Aerial"));
	SpawnPointAerial->SetupAttachment(RootComponent);
	SpawnPointAerial->ComponentTags.Add("Aerial");
	SpawnPointComponents.Add(SpawnPointAerial);

	SpawnPointHangar = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,TEXT("Hangar"));
	SpawnPointHangar->SetupAttachment(RootComponent);
	SpawnPointHangar->ComponentTags.Add("Hangar");
	SpawnPointComponents.Add(SpawnPointHangar);


	if (SpawnPointComponents.Num() > 0)
	{
		for (USceneComponent* SpawnPoint : SpawnPointComponents)
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

	SetActorHiddenInGame(true);
}

