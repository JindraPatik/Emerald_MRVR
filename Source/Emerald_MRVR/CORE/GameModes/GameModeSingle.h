#pragma once

#include "CoreMinimal.h"
#include "GameModeCommon.h"
#include "GameModeSingle.generated.h"

class UPowerUpSpawner;
class AUnitAIController;
class AAIPawn;
class AVRPlayerController;
class ATargetPoint;
class APlayerStart;
class UCrystalSpawnerComp;
class AVRPawn;

UCLASS()
class EMERALD_MRVR_API AGameModeSingle : public AGameModeCommon
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
		void StartGame() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Pawn")
		TSubclassOf<AVRPawn> VRPawnClass;

	UPROPERTY(EditDefaultsOnly, Category="Pawn")
		TSubclassOf<AVRPawn> SpectatorPawn;

	UPROPERTY(EditDefaultsOnly, Category="AI")
		TSubclassOf<AAIPawn> EnemyToSpawn;

	UPROPERTY(EditAnywhere, Category="Resources")
		UCrystalSpawnerComp* CrystalSpawner;

	UPROPERTY(EditAnywhere, Category="Resources")
		UPowerUpSpawner* PowerUpSpawner;

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
	AGameModeSingle();
	
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(Category="Spawning")
		FTransform FindMyPlayerStart();

	UPROPERTY()
		AAIPawn* EnemyPawn;
};
