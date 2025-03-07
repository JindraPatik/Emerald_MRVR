#include "PowerUpSpawner.h"
#include "Emerald_MRVR/CORE/GameModes/GameModeCommon.h"

class ASpawnPointCrystal;

UPowerUpSpawner::UPowerUpSpawner()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPowerUpSpawner::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	AGameModeCommon* AGameMode = Cast<AGameModeCommon>(World->GetAuthGameMode());
	if (!AGameMode)
	{
		return;
	}

	AGameMode->OnGameStartedDelegate.AddDynamic(this, &UPowerUpSpawner::StartSpawning);
	
}

void UPowerUpSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPowerUpSpawner::StartSpawning()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	float SpawnInterval = FMath::FRandRange(SpawnIntervalMin, SpawnIntervalMax);
	World->GetTimerManager().SetTimer(SpawningHandle, this, &UPowerUpSpawner::SpawnPowerUp, SpawnInterval, true);
}

void UPowerUpSpawner::SpawnPowerUp_Implementation()
{
	/* Implementation in BP */
}


