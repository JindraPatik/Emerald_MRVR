#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MilitaryBase.generated.h"

class UBuildingDataAsset;
class APC_MR_General;
class AUnit;
class UHealthComponent;
class UBuildingsModuleComponent;
class UDownScaleComponent;
class UBoxComponent;

UCLASS()
class EMERALD_MRVR_API AMilitaryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMilitaryBase();
	void SpawnResourcesWidget();

	UFUNCTION(Server, Reliable)
	void Server_SpawnResourcesWidget();

	void SpawnHealthWidget();

	UFUNCTION(Server, Reliable)
	void Server_SpawnHealthWidget();
	
	

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, Category = "Visuals")
	TObjectPtr<UStaticMeshComponent> BaseBody;

	UPROPERTY(EditAnywhere, Category = "BaseBody")
	TObjectPtr<UBoxComponent> BaseBox;

	UPROPERTY(VisibleDefaultsOnly, Category="Visuals")
	UDownScaleComponent* DownScaleComponent;

	// Building modules
	UPROPERTY(Replicated, EditAnywhere, Category = "Buildings")
	TObjectPtr<USceneComponent> Modules;

	UPROPERTY(Replicated, EditAnywhere, Category = "Buildings")
	TArray<UBuildingDataAsset*> BuildingModules;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite); 
	TArray<UBuildingsModuleComponent*> ReplicatedBuildingComponents;

	UPROPERTY()
	TMap<FName, UBuildingsModuleComponent*> BuildingComponentsMap;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category="Modules")
	UBuildingsModuleComponent* BuildingsModuleComponent;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	USceneComponent* SpawnPoint_Ground;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	USceneComponent* SpawnPoint_Air;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Buildings")
	TArray<UBuildingDataAsset*> AvailableBuildings;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<AActor> ResourcesWBP;

	UPROPERTY(Replicated)
	AActor* ResourcesWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<AActor> HealthWidget;

	UPROPERTY(Replicated)
	AActor* HealthWidgetInstance;
};
