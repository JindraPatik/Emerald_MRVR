#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MilitaryStation.generated.h"

class AVRPawn;
class UBuildingDataAsset;
class AVRPlayerController;
class AUnit;
class UHealthComponent;
class UBuildingsComponent;
class UDownScaleComponent;
class UBoxComponent;

UCLASS()
class EMERALD_MRVR_API AMilitaryStation : public AActor
{
	GENERATED_BODY()
	
public:	
	AMilitaryStation(const FObjectInitializer& ObjectInitializer);
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
		TObjectPtr<USceneComponent> StationRoot;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Visuals")
		TObjectPtr<UStaticMeshComponent> Body_Root;

	UPROPERTY(EditAnywhere, Category = "BaseBody")
		TObjectPtr<UBoxComponent> BaseBox;

	UPROPERTY(VisibleDefaultsOnly, Category="Visuals")
		UDownScaleComponent* DownScaleComponent;

	UPROPERTY(EditAnywhere, Category="Widgets")
		FVector ResourcesWidgetOffset;

	UPROPERTY(EditAnywhere, Category="Widgets")
		FVector HealthWidgetOffset;

	UPROPERTY(Replicated)
		TObjectPtr<APawn> VR_Pawn;

public:	

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		USceneComponent* SpawnPointGround;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		USceneComponent* SpawnPointAir;

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

	// Buildings 
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> PositionsRoot;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> MinePosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> PrisonPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> BankPosition;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> UVKSCPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> MicrosoftPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> BarracsPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> GaragePosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> LPlatformPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> MinecraftPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> HeliportPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> RunwayPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> AerialPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> HangarPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> RocketLauncherPosition;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Spawning")
		TArray<USceneComponent*> BuildingPositions;
};
