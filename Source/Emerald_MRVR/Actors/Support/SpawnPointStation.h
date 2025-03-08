 #pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPointStation.generated.h"

class UArrowComponent;
class UBillboardComponent;

UCLASS()
class EMERALD_MRVR_API ASpawnPointStation : public AActor
{
	GENERATED_BODY()

public:
	ASpawnPointStation(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<UArrowComponent> ArrowComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointMine;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointPrison;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointVault;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointPit;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointSilo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointOutpost;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointFort;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointBastion;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointPlatform;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointDepot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointHelipad;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointRunway;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointAerial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointHangar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<UBillboardComponent> BillboardComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flags")
		bool bIsReversed =  false;

	/* If spawning incorrectly, switch Indexes in the world */
	UPROPERTY(EditInstanceOnly, Category="Mechanics")
		uint8 index = 0;

	TArray<USceneComponent*> SpawnPointComponents;
};
