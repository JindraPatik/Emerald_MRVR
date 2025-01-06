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
	float GetDistanceBetweenCrystalSpawners() const;
	float GetMyDistanceFromCrystal(FVector CrystalLocation) const;
	void SpawnHarvester(UMilitaryBaseComp* MilitaryBaseComp);
	void GetAvailableUnits();
	void GetAvailableModules();
	void SpawnUnit(UBuildingDataAsset* ModuleData, bool bIsFlying);
	void SpawnRandomUnit();
	void HandleRandomSpawn();
	

	UPROPERTY()
		TObjectPtr<AGameState> AEK_GameStateInst;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		TObjectPtr<UBuildingDataAsset> MineModule;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float DistanceToCrystalTolerance = 2;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float MaxSimulatedDelayToSpawnHarvester = 0.8f;

	UPROPERTY(EditDefaultsOnly, Category="Spawning") // unused
		float MaxSimulatedDelayToSpawnreactUnit = 1.8f;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float ProbabilityFactorToSpawnReactUnit = 65.f;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float RandomSpawnMin = 3.f;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float RandomSpawnMax = 10.f;

	FTimerHandle RandomSpawn_Handle;


	UPROPERTY(VisibleAnywhere, Category="Spawning")
		TArray<UBuildingDataAsset*> AvailableGroundUnits;

	UPROPERTY(VisibleAnywhere, Category="Spawning")
		TArray<UBuildingDataAsset*> AvailableFlyingUnits;

	UFUNCTION()
		void OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst);
	 void ChooseOptimalUnit(AUnit* Unit, UMilitaryBaseComp* MilitaryBaseComp, TArray<UBuildingDataAsset*> Availables);

	UPROPERTY()
		UBuildingDataAsset* CheapestStronger; // Temp variable for Cheapest stronger Unit

	UPROPERTY()
		UBuildingDataAsset* CheapestSame; // Temp variable for Cheapest stronger Unit

public:
	UFUNCTION(BlueprintCallable, Category="Events")
		void OnUnitOccured(AUnit* Unit, AActor* Owner);
	
};
