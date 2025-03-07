#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModeCommon.generated.h"

class AEKGameState;
class AUnit;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEndedSignature, APawn*, Looser);

UCLASS()
class EMERALD_MRVR_API AGameModeCommon : public AGameMode
{
	GENERATED_BODY()

protected:
	AGameModeCommon(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	void StartCountdown();
	void DecreaseCounter();
	void SpawnCountDownWidgetActor();
	void StopAllUnits();
	
	FTimerHandle CountDownHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
		TSubclassOf<AActor> CountDownWidgetActor;

	UPROPERTY()
		AActor* CountDownWidgetActorInstance;
	
public:
	virtual void StartGame();
	virtual void EndGame(APawn* InPawn);

	TObjectPtr<AEKGameState> GameState;

	FOnGameStartedSignature OnGameStartedDelegate;
	FOnGameEndedSignature OnGameEndedDelegate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CountDown")
		int32 CountDownValue = 3;
};
