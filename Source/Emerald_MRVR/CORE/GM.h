// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartedSignature);

UCLASS()
class EMERALD_MRVR_API AGM : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	FTimerHandle CountDownHandle;
	void StartCountdown();
	void DecreaseCounter();
	void SpawnCountDownWidgetActor();
	virtual void StartGame();
	bool bGameHasStarted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
		TSubclassOf<AActor> CountDownWidgetActor;

	UPROPERTY()
		AActor* CountDownWidgetActorInstance;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CountDown")
		int32 CountDownValue = 5;

	FOnGameStartedSignature OnGameStartedDelegate;
};
