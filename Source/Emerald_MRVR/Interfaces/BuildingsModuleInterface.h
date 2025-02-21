// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BuildingsModuleInterface.generated.h"

class AMilitaryBase;
class UBuildingsModuleComponent;
class AMR_General;
class AUnit;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UBuildingsModuleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EMERALD_MRVR_API IBuildingsModuleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UBuildingsModuleComponent* SelectModule();
	virtual void DeployUnit(AMR_General* InPawn, AMilitaryBase* BaseInstance);
};
