// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GM.h"
#include "GameFramework/GameMode.h"
#include "Gamemode_Single.generated.h"


class AUnitAIController;
class AAIPawn;
class APC_MR_General;
class ATargetPoint;
class APlayerStart;
class UCrystalSpawnerComp;
class AMR_General;

UCLASS()
class EMERALD_MRVR_API AGamemode_Single : public AGM
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
		void StartGame() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Pawn")
		TSubclassOf<AMR_General> PawnToSpawn;

	UPROPERTY(EditDefaultsOnly, Category="AI")
		TSubclassOf<AAIPawn> EnemyToSpawn;

	UPROPERTY(EditAnywhere, Category="Resources")
		UCrystalSpawnerComp* CrystalSpawner;

	UPROPERTY(BlueprintReadOnly, Category="Spawning")
		TArray<APlayerStart*> AllPlayerStarts;

	UFUNCTION()
		void FindAllPlayerStarts();

	UFUNCTION(Category="Spawning")
		void SpawnPlayer(APlayerController* PlayerController);

	UFUNCTION(Category="Spawning")
		void SpawnEnemyAI();

	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	
public:
	AGamemode_Single();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UPROPERTY(BlueprintReadOnly, Category="Spawning")
		TArray<ATargetPoint*> TargetPoints;
	
	UFUNCTION(Category="Spawning")
		FTransform FindMyPlayerStart();

	UPROPERTY()
		AAIPawn* EnemyPawn;
	
	
};
