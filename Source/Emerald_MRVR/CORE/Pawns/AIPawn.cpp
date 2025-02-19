#include "AIPawn.h"

#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/Components/AI/AIComponent.h"
#include "Emerald_MRVR/Components/Health/HealthComponent.h"
#include "Emerald_MRVR/Components/MilitaryBase/MilitaryStationComp.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"

AAIPawn::AAIPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>("Resources");
	MilitaryStationComp = CreateDefaultSubobject<UMilitaryStationComp>("MilitaryStationComp");
	AIComponent = CreateDefaultSubobject<UAIComponent>("AI_Component");
}

void AAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
	MilitaryStationComp->SpawnMilitaryStation(this);
	MilitaryStationComp->SpawnBuildings(this);
	AIComponent->GetAvailableAttackingUnits();;

	AEK_GameStateInst = Cast<AEKGameState>(GetWorld()->GetGameState());
	if (AEK_GameStateInst)
	{
		AGameModeCommon* GM = Cast<AGameModeCommon>(AEK_GameStateInst->AuthorityGameMode);
		if (GM)
		{
			GM->OnGameStartedDelegate.AddDynamic(this, &AAIPawn::StartGame);
			GM->OnGameEndedDelegate.AddDynamic(this, &AAIPawn::EndGame);
		}
	}
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

