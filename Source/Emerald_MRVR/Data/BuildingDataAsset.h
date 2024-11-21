#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/EKG_Enums.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

class UUnitDataAsset;

UCLASS()
class EMERALD_MRVR_API UBuildingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Info")
	FName BuildingName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
	EKGBuildings BuildingType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Building")
	TObjectPtr<UUnitDataAsset> UnitToSpawn;
};
