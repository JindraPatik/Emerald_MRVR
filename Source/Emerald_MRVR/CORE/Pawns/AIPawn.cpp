#include "AIPawn.h"

#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/Components/AI/AIComponent.h"
#include "Emerald_MRVR/Components/Health/HealthComponent.h"
#include "Emerald_MRVR/Components/MilitaryBase/MilitaryBaseComp.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"

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
	MilitaryBaseComp->SpawnBuildings(this);
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

