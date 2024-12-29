#pragma once

#include "CoreMinimal.h"
#include "MilitaryBaseComp.h"
#include "Components/ActorComponent.h"
#include "AI_Component.generated.h"


class AModuleActor;
class UBuildingDataAsset;
class AGameState;
class ACrystal;
class AEKGameState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UAI_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAI_Component();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	float GetDistanceBetweenCrystalSpawners() const;
	float GetMyDistanceFromCrystal(FVector CrystalLocation) const;
	void SpawnHarvester(UMilitaryBaseComp* MilitaryBaseComp);

	UPROPERTY()
		TObjectPtr<AGameState> AEK_GameStateInst;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		TObjectPtr<UBuildingDataAsset> MineModule;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float DistanceToCrystalTolerance = 2;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float MaxSimulatedDelayToSpawnHarvester = 0.8f;

	UFUNCTION()
	void OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
