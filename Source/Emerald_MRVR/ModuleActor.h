#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/BuildingsModuleInterface.h"
#include "Interfaces/InteractionInterface.h"
#include "ModuleActor.generated.h"

class UDownScaleComponent;
class UBuildingDataAsset;
class AMR_General;

UCLASS()
class EMERALD_MRVR_API AModuleActor : public AActor, public IBuildingsModuleInterface, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AModuleActor();
	AMR_General* General;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnBuildingsDataChanged();
	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TObjectPtr<UDownScaleComponent> DownScaleComponent;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
	TObjectPtr<UStaticMeshComponent> ModuleMeshRoot;

	UPROPERTY(ReplicatedUsing=OnBuildingsDataChanged, VisibleAnywhere, Category="Data")
	TObjectPtr<UBuildingDataAsset> BuildingDataAsset = nullptr;

	UPROPERTY(Replicated)
	UMaterialInterface* OriginalMaterial;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Color")
	UMaterialInterface* HoverMaterial;

};
