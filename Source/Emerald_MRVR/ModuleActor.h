#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BuildingsModuleInterface.h"
#include "ModuleActor.generated.h"

class UDownScaleComponent;
class UBuildingDataAsset;
class AMR_General;
class ACooldownActor;

UCLASS()
class EMERALD_MRVR_API AModuleActor : public AActor, public IBuildingsModuleInterface
{
	GENERATED_BODY()
	
public:	
	AModuleActor();

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
		TObjectPtr<UStaticMeshComponent> ModuleMeshRoot;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Data")
		TObjectPtr<UBuildingDataAsset> BuildingDataAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
		TSubclassOf<AActor> InfoWidgetActor;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
		TObjectPtr<USceneComponent> InfoWidgetSpawnPoint;


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
