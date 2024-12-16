// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;

UCLASS()
class EMERALD_MRVR_API AUnitAIController : public AAIController
{
	GENERATED_BODY()

public:
	AUnitAIController();
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	// behaviour tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent { nullptr };

	// blackboard component, uses the blackboard owned by the behavior tree
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

private:
	UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTree;

	

	
};
