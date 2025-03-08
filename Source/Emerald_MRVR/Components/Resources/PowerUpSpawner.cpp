#include "PowerUpSpawner.h"

#include "Emerald_MRVR/Actors/Resources/PowerUp/PowerUp.h"
#include "Emerald_MRVR/CORE/GameModes/GameModeCommon.h"

class ASpawnPointCrystal;

UPowerUpSpawner::UPowerUpSpawner(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

void UPowerUpSpawner::SelectRandomPowerUp()
{

	if (PowerUpsToSpawn.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PowerUpsToSpawn.Num() - 1);
		RandomPowerUp = PowerUpsToSpawn[RandomIndex];
	}
	
}

void UPowerUpSpawner::SpawnPowerUp()
{
	SelectRandomPowerUp();
	
	/* Find location in BP with MRUK */
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!RandomPowerUp)
	{
		return;
	}

	SpawnLocation = FVector(0.f, 0.f, 300.f);

	FActorSpawnParameters SpawnParameters;
	PowerUpInstance = World->SpawnActor<APowerUp>(RandomPowerUp, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
}


