#include "AIPawn.h"

#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/Components/AI/AIComponent.h"
#include "Emerald_MRVR/Components/Health/HealthComponent.h"
#include "Emerald_MRVR/Components/MilitaryStation/MilitaryStationComp.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"

AAIPawn::AAIPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>("Resources");
	MilitaryStationComp = CreateDefaultSubobject<UMilitaryStationComp>("MilitaryBaseComp");
	AIComponent = CreateDefaultSubobject<UAIComponent>("AI_Component");
}

void AAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
	MilitaryStationComp->SpawnMilitaryStation(this);
	MilitaryStationComp->SpawnBuildings(this);
	AIComponent->GetAvailableAttackingUnits();;

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

void AAIPawn::AddPowerUp(APowerUp* InPowerUp)
{
	AvailablePowerUps.Add(InPowerUp);
	UE_LOG(LogTemp, Warning, TEXT("AIPawn::AddPowerUp added"));
}

