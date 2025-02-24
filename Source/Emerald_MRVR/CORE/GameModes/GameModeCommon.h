#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModeCommon.generated.h"

class AUnit;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEndedSignature, APawn*, Looser);

UCLASS()
class EMERALD_MRVR_API AGameModeCommon : public AGameMode
{
	GENERATED_BODY()

protected:
	
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
	virtual void EndGame(APawn* Looser);

	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
		bool bGameHasStarted = false;						//pb: proc mit bGameHasStarted a bGameHasEnded? muze vzniknout nekonzistentni stav. 
															//	  Lepsi je mit napr. Enum E_MatchState {WaitingForPlayers, Ready, Started, InProgress, Ended}
															//	  Nejlepe bych ale tento stav presunul do GameState - probereme Tvuj zamer, proc to mas zde

	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
		bool bGameHasEnded = false;
	
	FOnGameStartedSignature OnGameStartedDelegate;
	FOnGameEndedSignature OnGameEndedDelegate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CountDown")
		int32 CountDownValue = 3;
};
