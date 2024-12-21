// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleAIController.h"

#include "BehaviorTree/BehaviorTree.h"


void ASingleAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
}

ASingleAIController::ASingleAIController()
{
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>("BehaviorTree");
}
