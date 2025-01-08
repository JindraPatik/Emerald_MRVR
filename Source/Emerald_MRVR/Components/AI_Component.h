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

enum EUnitFightStatus
{
	EIsAttacking,
	EIsDefending
};

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
	AUnit* SpawnUnit(UBuildingDataAsset* ModuleData, bool bIsFlying);
	void SpawnRandomUnit();
	void HandleRandomSpawn();

	UFUNCTION()
		void StartGame();
	
	UFUNCTION()
		void TryToDefend(UMilitaryBaseComp* MilitaryBaseComp, TArray<UBuildingDataAsset*> Availables);

	float DefendingAgainstValue;

	FTimerHandle RandomSpawn_Handle;
	FTimerHandle Defending_Handle;
	FTimerDelegate Defending_Delegate;
	EUnitFightStatus FightStatus;

	UPROPERTY()
		AUnit* UndefendedUnit;

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

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float DefendingRate = 1.f;

public:
	UFUNCTION(BlueprintCallable, Category="Events")
		void OnUnitOccured(AUnit* Unit, AActor* Owner);
	
};
