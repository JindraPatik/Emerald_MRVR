// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleAIController.h"

#include "BehaviorTree/BehaviorTree.h"


ASingleAIController::ASingleAIController()
{
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>("BehaviorTree");
}
