// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeCommon.h"
#include "GameFramework/GameMode.h"
#include "GameMode_Single.generated.h"


class AUnitAIController;
class AAIPawn;
class AVRPlayerController;
class ATargetPoint;
class APlayerStart;
class UCrystalSpawnerComp;
class AVRPawn;

UCLASS()
class EMERALD_MRVR_API AGameMode_Single : public AGameModeCommon				//pb: konzistence nazvu... AGameMode_Single
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
		void StartGame() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Pawn")
		TSubclassOf<AVRPawn> Mr_General_Class;

	UPROPERTY(EditDefaultsOnly, Category="Pawn")
		TSubclassOf<AVRPawn> SpectatorPawn;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
		bool bIsSpectator = false;
	
public:
	AGameMode_Single();
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
