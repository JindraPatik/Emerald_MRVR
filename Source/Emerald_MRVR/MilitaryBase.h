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
		TObjectPtr<USceneComponent> BaseRoot;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Visuals")
		TObjectPtr<UStaticMeshComponent> SM_Root;

	UPROPERTY(EditAnywhere, Category = "BaseBody")
		TObjectPtr<UBoxComponent> BaseBox;

	UPROPERTY(VisibleDefaultsOnly, Category="Visuals")
		UDownScaleComponent* DownScaleComponent;

	UPROPERTY(EditAnywhere, Category="Widgets")
		FVector ResourcesWidgetOffset;

	UPROPERTY(EditAnywhere, Category="Widgets")
		FVector HealthWidgetOffset;

	UPROPERTY(Replicated)
		APawn* General;
	

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

	// Modules 
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Positions_Root;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Mine_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Prison_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Bank_Position;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> UVKSC_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Microsoft_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Barracs_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Garage_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> LPlatform_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Minecraft_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Heliport_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Runway_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Aerial_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> Hangar_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TObjectPtr<USceneComponent> RocketLauncher_Position;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Spawning")
	TArray<USceneComponent*> ModulePositions;
};
