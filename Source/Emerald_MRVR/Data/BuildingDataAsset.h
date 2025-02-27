#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Support/EmeraldEnums.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

class AUnit;
class UUnitDataAsset;
class ABuilding;

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
		UStaticMesh* BuildingBody;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		TSubclassOf<ABuilding> BuildingClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Unit")
		TObjectPtr<UUnitDataAsset> UnitToSpawnData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Unit")
		TSubclassOf<AUnit> UnitToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
		UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
		UMaterialInterface* HoverMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		float Cooldown = 0.f;

	
};
