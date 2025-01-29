// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crystal.generated.h"

class UDownScaleComponent;
class UBoxComponent;

UCLASS()
class EMERALD_MRVR_API ACrystal : public AActor
{
	GENERATED_BODY()
	
public:	
	ACrystal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Body")
		USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly, Category="Body")
		UStaticMeshComponent* SM_Crystal;

	UPROPERTY(EditDefaultsOnly, Category="Body")
		UStaticMeshComponent* SM_Bottom;
	
	UPROPERTY(EditDefaultsOnly, Category="Body")
		UBoxComponent* CrystalBox;

	UPROPERTY(EditDefaultsOnly, Category="Scale")
		UDownScaleComponent* DownScaleComponent;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
		float CrystalValue;
	
	
};
