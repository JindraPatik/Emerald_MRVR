// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_MR_General.generated.h"

class AMR_General;

UCLASS()
class EMERALD_MRVR_API APC_MR_General : public APlayerController
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	AMR_General* General;

public:
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "CORE")
	APC_MR_General* OtherPlayerPC;
};
