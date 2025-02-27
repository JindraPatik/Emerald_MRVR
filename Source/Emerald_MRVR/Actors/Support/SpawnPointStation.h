#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPointStation.generated.h"

class UArrowComponent;

UCLASS()
class EMERALD_MRVR_API ASpawnPointStation : public AActor
{
	GENERATED_BODY()

public:
	ASpawnPointStation();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<UBillboardComponent> BillboardComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flags")
		bool bIsReversed =  false;

	TArray<USceneComponent*> SpawnPointComponents;
};
