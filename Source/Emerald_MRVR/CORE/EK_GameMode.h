// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EK_GameMode.generated.h"

class ATargetPoint;
/**
 * 
 */
UCLASS()
class EMERALD_MRVR_API AEK_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<ATargetPoint*> TargetPoints;

	void GetAllTargetpoints();
	
};
