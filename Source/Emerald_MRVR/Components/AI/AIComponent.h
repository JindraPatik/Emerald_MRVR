#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Components/MilitaryBase/MilitaryStationComp.h"
#include "Components/ActorComponent.h"
#include "AIComponent.generated.h"


class ABuilding;
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
class EMERALD_MRVR_API UAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAIComponent();
	void GetAvailableAttackingUnits();

protected:
	virtual void BeginPlay() override;
	float GetDistanceBetweenCrystalSpawners() const;
	float GetMyDistanceFromCrystal(FVector CrystalLocation) const;
	void SpawnHarvester(UMilitaryStationComp* MilitaryStationComp);
	AUnit* SpawnUnit(ABuilding* Building);
	void SpawnRandomUnit();
	
	UFUNCTION()
		void TryToDefend(UMilitaryStationComp* MilitaryStationComp, TArray<ABuilding*> Availables);

	UPROPERTY()
		float DefendingAgainstValue;

	FTimerHandle RandomSpawn_Handle;
	FTimerHandle Defending_Handle;
	FTimerHandle CD_Handle;
	FTimerDelegate Defending_Delegate;
	EUnitFightStatus FightStatus;
	bool bSpawningEnabled = true;

	UPROPERTY()
		TObjectPtr<APawn> AI_Pawn;

	UPROPERTY()
		TObjectPtr<AUnit> UndefendedUnit;

	UPROPERTY()
		TObjectPtr<AGameState> AEK_GameStateInst;

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		TObjectPtr<UBuildingDataAsset> MineBuilding;

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
		TArray<ABuilding*> AvailableGroundUnits;

	UPROPERTY(VisibleAnywhere, Category="Spawning")
		TArray<ABuilding*> AvailableFlyingUnits;

	UFUNCTION()
		void OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst);
	
	UFUNCTION()
		void ChooseOptimalUnit(AUnit* AttackerUnit, UMilitaryStationComp* MilitaryStationComp, TArray<ABuilding*> Availables);

	UPROPERTY()
		TObjectPtr<ABuilding> CheapestStronger; // Temp variable for Cheapest stronger Unit

	UPROPERTY()
		TObjectPtr<ABuilding> CheapestSame; // Temp variable for Cheapest stronger Unit

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
		float DefendingRate = 1.f;

	UFUNCTION()
		void EnableSpawning();
	
	UFUNCTION()
		void Cooldown(float CD_Time);

public:
	UFUNCTION(BlueprintCallable, Category="Events")
		void OnUnitOccured(AUnit* Unit, AActor* Owner);

	void HandleRandomSpawn();

	bool bIsReversed;
	
};
