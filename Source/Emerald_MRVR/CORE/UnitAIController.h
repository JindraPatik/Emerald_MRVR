// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"

/**
 * 
 */
UCLASS()
class EMERALD_MRVR_API AUnitAIController : public AAIController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

	
};
