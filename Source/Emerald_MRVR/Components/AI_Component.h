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
	void GetAvailableAttackingUnits();

protected:
	virtual void BeginPlay() override;
	float GetDistanceBetweenCrystalSpawners() const;
	float GetMyDistanceFromCrystal(FVector CrystalLocation) const;
	void SpawnHarvester(UMilitaryBaseComp* MilitaryBaseComp);
	void GetAvailableModules();
	AUnit* SpawnUnit(AModuleActor* Module);
	void SpawnRandomUnit();




	
	UFUNCTION()
		void TryToDefend(UMilitaryBaseComp* MilitaryBaseComp, TArray<AModuleActor*> Availables);

	float DefendingAgainstValue;

	FTimerHandle RandomSpawn_Handle;
	FTimerHandle Defending_Handle;
	FTimerHandle CD_Handle;
	FTimerDelegate Defending_Delegate;
	EUnitFightStatus FightStatus;
	bool bSpawningEnabled = true;

	UPROPERTY()
		APawn* AI_Pawn;

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
		TArray<AModuleActor*> AvailableGroundUnits;

	UPROPERTY(VisibleAnywhere, Category="Spawning")
		TArray<AModuleActor*> AvailableFlyingUnits;

	UFUNCTION()
		void OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst);
	
	UFUNCTION()
		void ChooseOptimalUnit(AUnit* AttackerUnit, UMilitaryBaseComp* MilitaryBaseComp, TArray<AModuleActor*> Availables);

	UPROPERTY()
		AModuleActor* CheapestStronger; // Temp variable for Cheapest stronger Unit

	UPROPERTY()
		AModuleActor* CheapestSame; // Temp variable for Cheapest stronger Unit

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
	
};
