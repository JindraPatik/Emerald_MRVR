// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEndedSignature, APawn*, Looser);

UCLASS()
class EMERALD_MRVR_API AGM : public AGameMode
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;
	void StartCountdown();
	void DecreaseCounter();
	void SpawnCountDownWidgetActor();
	
	FTimerHandle CountDownHandle;
	bool bGameHasStarted = false;
	bool bGameHasEnded = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
		TSubclassOf<AActor> CountDownWidgetActor;

	UPROPERTY()
		AActor* CountDownWidgetActorInstance;
	
public:
	virtual void StartGame();
	virtual void EndGame(APawn* Looser);
	
	FOnGameStartedSignature OnGameStartedDelegate;
	FOnGameEndedSignature OnGameEndedDelegate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CountDown")
		int32 CountDownValue = 5;
};
