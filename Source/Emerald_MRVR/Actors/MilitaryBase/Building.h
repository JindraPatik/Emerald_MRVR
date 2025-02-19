#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

class UDownScaleComponent;
class UBuildingDataAsset;
class AVRPawn;
class ACooldownActor;


//pb: doporucoval bych jiny nazev nez ModuleActor, protoze pojem Module se v UE pouziva pro herni moduly (architektonicky prvek), viz. https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-modules
UCLASS()
class EMERALD_MRVR_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuilding();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void DisableInfoWidget();
	void EnableInfoWidget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
		TSubclassOf<ACooldownActor> CooldownWidgetClass; // priradit v Editoru!

	UPROPERTY()
		TObjectPtr<ACooldownActor> CooldownWidgetInstance;

	UFUNCTION()
		void SpawnCooldownWidget();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
		TObjectPtr<UDownScaleComponent> DownScaleComponent;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<UStaticMeshComponent> BuildingMeshRoot;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Data")
		TObjectPtr<UBuildingDataAsset> BuildingDataAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
		TSubclassOf<AActor> InfoWidgetActor;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
		TObjectPtr<USceneComponent> InfoWidgetSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<UMaterialInterface> OverlayMaterial;

	UFUNCTION(BlueprintCallable, Category="Visuals")
		void SetOverlayMaterial();

	UFUNCTION(BlueprintCallable, Category="Visuals")
		void RemoveOverlayMaterial();
	
	


	UPROPERTY()
		AActor* InfoWidgetActorInst;
	
	UPROPERTY()
		bool bSpawningEnabled = true;

	UFUNCTION()
		void EnableSpawning();
	
	UFUNCTION()
		void Cooldown(float CD_Time);
	
	UFUNCTION()
		void SpawnInfoWidget();

	FTimerHandle CD_Handle;

	UFUNCTION()
		void SetInfoWidgetStats(AActor* WidgetActor);

	UPROPERTY(EditAnywhere, Category="Widgets")
		float InfoWidgetHeight = 25.f;

	UPROPERTY(EditAnywhere, Category="Widgets")
		float CooldownWidgetHeight = 15.f;

	float CooldownDuration;
	float ElapsedTime;
};
