// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HarvesterComponent.generated.h"

class UBoxComponent;
class AUnit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UHarvesterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHarvesterComponent();

	UFUNCTION()
	void OnBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	AUnit* Unit;
	
	UPROPERTY(BlueprintReadWrite, Category="Collision")
	UBoxComponent* BoxComponent;

	void CollectCrystal(AActor* HittedActor);

	UFUNCTION(Server, Reliable)
	void Server_CollectCrystal(AActor* HitActor);

	void DeliverCrystal(AActor* HitActor);

	UFUNCTION(Server, Reliable)
	void Server_DeliverCrystal(AActor* HitActor);
public:	

	bool bIsLoaded = false;
	float HarvestedValue;
		
	
};
