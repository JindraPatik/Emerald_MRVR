// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_MR_General.generated.h"

class AMR_General;
/**
 * 
 */
UCLASS()
class EMERALD_MRVR_API APC_MR_General : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(Server, Reliable, Category="Spawning")
	void Server_SpawnPlayer();

	UFUNCTION(BlueprintCallable, Category="Core")
	void SetOtherPlayerPC();
	
	UFUNCTION(BlueprintCallable, Category="Core")
	void SetOtherPlayerPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Core")
	APC_MR_General* OtherPlayerPC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Core")
	AMR_General* OtherPlayerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Core")
	bool bIsReferencesSet = false;
	
};
