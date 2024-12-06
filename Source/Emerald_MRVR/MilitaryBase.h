#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MilitaryBase.generated.h"

class AMR_General;
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Visuals")
	TObjectPtr<UStaticMeshComponent> BaseBody;

	UPROPERTY(EditAnywhere, Category = "BaseBody")
	TObjectPtr<UBoxComponent> BaseBox;

	UPROPERTY(VisibleDefaultsOnly, Category="Visuals")
	UDownScaleComponent* DownScaleComponent;

	UPROPERTY(Replicated)
	AMR_General* General;

	virtual void PostInitProperties() override;
	

public:	

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	USceneComponent* SpawnPoint_Ground;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	USceneComponent* SpawnPoint_Air;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<AActor> ResourcesWBP;

	UPROPERTY(Replicated)
	AActor* ResourcesWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<AActor> HealthWidget;

	UPROPERTY(Replicated)
	AActor* HealthWidgetInstance;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category="Visuals")
	UMaterialInterface* OriginalMaterial;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Visuals")
	UMaterialInterface* HoveredMaterial;
};
