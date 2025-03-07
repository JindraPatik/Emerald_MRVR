#pragma once

#include "CoreMinimal.h"
#include "ActorComponent.h"
#include "PowerUpSpawner.generated.h"

class APowerUp;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EMERALD_MRVR_API UPowerUpSpawner : public UActorComponent
{
	GENERATED_BODY()

public:
	UPowerUpSpawner();

protected:
	virtual void BeginPlay() override;

	/* Assign in Editor */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
		TArray<TSubclassOf<APowerUp>> PowerUpsToSpawn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawning", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APowerUp> RandomPowerUp;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float SpawnIntervalMin = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float SpawnIntervalMax = 10.f;

	UPROPERTY(VisibleInstanceOnly, Category="Spawning")
		TObjectPtr<APowerUp> PowerUpInstance;

	FTimerHandle SpawningHandle;
	
	UFUNCTION()
		void StartSpawning();

	UFUNCTION()
		void SelectRandomPowerUp();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
		void SpawnPowerUp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		FVector SpawnLocation;
};
