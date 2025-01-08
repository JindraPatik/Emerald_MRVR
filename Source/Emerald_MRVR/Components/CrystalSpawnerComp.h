#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrystalSpawnerComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCrystalSpawnedSignature, FVector, SpawnedLoc, ACrystal*, CrystalInstance);

class ACrystal;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UCrystalSpawnerComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCrystalSpawnerComp();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Crystal")
		TSubclassOf<ACrystal> CrystalToSpawn;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float SpawnIntervalMin = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float SpawnIntervalMax = 10.f;

	UPROPERTY(VisibleInstanceOnly, Category="Crystal")
		TObjectPtr<ACrystal> CrystalInst;

	FTimerHandle SpawningHandle;

	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnCrystal();
	void StartSpawning();

	UFUNCTION()
	void StopSpawning(APawn* Looser);

	

	FOnCrystalSpawnedSignature OnCrystalSpawnedDelegate;
	
};
