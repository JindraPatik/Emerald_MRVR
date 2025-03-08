#include "AIPawn.h"

#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/Components/AI/AIComponent.h"
#include "Emerald_MRVR/Components/Health/HealthComponent.h"
#include "Emerald_MRVR/Components/MilitaryStation/MilitaryStationComponent.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"

AAIPawn::AAIPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = ObjectInitializer.CreateDefaultSubobject<UHealthComponent>(this,"Health");
	ResourcesComponent = ObjectInitializer.CreateDefaultSubobject<UResourcesComponent>(this,"Resources");
	MilitaryStationComponent = ObjectInitializer.CreateDefaultSubobject<UMilitaryStationComponent>(this,"MilitaryBaseComp");
	AIComponent = ObjectInitializer.CreateDefaultSubobject<UAIComponent>(this,"AI_Component");
}

void AAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
	MilitaryStationComponent->SpawnMilitaryStation(this);
	MilitaryStationComponent->SpawnBuildings(this);
	AIComponent->GetAvailableAttackingUnits();
	

	GameStateInstance = Cast<AEKGameState>(GetWorld()->GetGameState());
	if (!GameStateInstance)
	{
		return;
	}
	
	AGameModeCommon* GM = Cast<AGameModeCommon>(GameStateInstance->AuthorityGameMode);
	if (!GM)
	{
		return;
	}
	GM->OnGameStartedDelegate.AddDynamic(this, &AAIPawn::StartGame);
	GM->OnGameEndedDelegate.AddDynamic(this, &AAIPawn::EndGame);
}

void AAIPawn::StartGame()
{
	AIComponent->HandleRandomSpawn();
	ResourcesComponent->StartGrowResources();
}

void AAIPawn::EndGame(APawn* Looser)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(AIComponent);
}

void AAIPawn::Tick(float DeltaTime)
{

}
