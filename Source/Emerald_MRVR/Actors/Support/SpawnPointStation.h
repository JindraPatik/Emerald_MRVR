#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPointStation.generated.h"

UCLASS()
class EMERALD_MRVR_API ASpawnPointStation : public AActor
{
	GENERATED_BODY()

public:
	ASpawnPointStation();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointMine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointPrison;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointVault;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointPit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointSilo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointOutpost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointFort;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointBastion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointPlatform;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointDepot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointHelipad;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointRunway;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointAerial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<USceneComponent> SpawnPointHangar;

	TArray<USceneComponent*> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<UBillboardComponent> BillboardComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flags")
		bool bIsReversed =  false;
	
};
