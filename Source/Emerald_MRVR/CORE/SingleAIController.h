// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SingleAIController.generated.h"

/**
 * 
 */
UCLASS()
class EMERALD_MRVR_API ASingleAIController : public AAIController
{
	GENERATED_BODY()

private:
	ASingleAIController();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
		UBehaviorTree* BehaviorTree;
	
	
};
