// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VRPlayerController.generated.h"

class AVRPawn;

UCLASS()
class EMERALD_MRVR_API AVRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	AVRPawn* VR_Pawn;

public:
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "CORE")
	AVRPlayerController* OtherPlayerPC;
};
