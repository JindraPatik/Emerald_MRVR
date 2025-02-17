#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Support/EKG_Enums.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

class AUnit;
class UUnitDataAsset;
class AModuleActor;

UCLASS()
class EMERALD_MRVR_API UBuildingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Info")
		FName BuildingName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
		EKGBuildings BuildingType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
		UStaticMesh* SM_Building;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		TSubclassOf<AModuleActor> ModuleClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Unit")
		TObjectPtr<UUnitDataAsset> UnitToSpawnData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Unit")
		TSubclassOf<AUnit> UnitToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
		UMaterialInterface* BaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
		UMaterialInterface* HoverMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		float Cooldown = 0.f;

	
};
