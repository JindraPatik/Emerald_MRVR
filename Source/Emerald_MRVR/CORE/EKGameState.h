// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "EKGameState.generated.h"

UENUM(BlueprintType)
enum class E_MatchState : uint8
{
	E_WaitingForPlayers UMETA(DisplayName = "Waiting for players"),
	E_Ready				UMETA(DisplayName = "Ready"),
	E_Started			UMETA(DisplayName = "Started"),
	E_InProgress 		UMETA(DisplayName = "In progress"),
	E_Ended				UMETA(DisplayName = "Ended"),
};
/**
 * 
 */
UCLASS()
class EMERALD_MRVR_API AEKGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CORE")
		E_MatchState CurrentMatchState;
	
};
