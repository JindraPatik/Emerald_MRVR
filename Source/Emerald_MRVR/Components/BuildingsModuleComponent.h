#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Emerald_MRVR/Interfaces/BuildingsModuleInterface.h"
#include "BuildingsModuleComponent.generated.h"


class UBuildingDataAsset;
class UStaticMeshComponent;
class UBoxComponent; // ?? Uvidime

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UBuildingsModuleComponent : public USceneComponent, public IBuildingsModuleInterface
{
	GENERATED_BODY()

public:	
	UBuildingsModuleComponent();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Komponenta pro statický mesh
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Building")
	UStaticMeshComponent* ModuleMesh;

	// Data Asset pro specifické informace o budově
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category="Building")
	UBuildingDataAsset* BuildingDataAsset;

	UPROPERTY(ReplicatedUsing=OnRep_SetInitialMaterial, VisibleDefaultsOnly, Category="Visuals")
	UMaterialInterface* BaseMaterial;

	UPROPERTY(ReplicatedUsing=OnRep_OnHoverMaterialChanged, VisibleDefaultsOnly, Category="Visuals")
	UMaterialInterface* HoverMaterial;

	UFUNCTION()
	void OnRep_SetInitialMaterial();

	UFUNCTION()
	void OnRep_OnHoverMaterialChanged();

	virtual void OnModuleHovered() override;
	virtual void OnModuleUnHovered() override;
	
	
};
