#include "AIPawn.h"

#include "EKGameState.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Emerald_MRVR/Components/AIComponent.h"
#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/Components/MilitaryBaseComp.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"

AAIPawn::AAIPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>("Resources");
	MilitaryBaseComp = CreateDefaultSubobject<UMilitaryBaseComp>("MilitaryBaseComp");
	AI_Component = CreateDefaultSubobject<UAIComponent>("AI_Component");
}

void AAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
	MilitaryBaseComp->SpawnMilitaryBase(this);
	MilitaryBaseComp->SpawnModules(this);
	AI_Component->GetAvailableAttackingUnits();;

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
	AI_Component->HandleRandomSpawn();
	ResourcesComponent->StartGrowResources();
}

void AAIPawn::EndGame(APawn* Looser)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(AI_Component);
}

void AAIPawn::Tick(float DeltaTime)
{

}

