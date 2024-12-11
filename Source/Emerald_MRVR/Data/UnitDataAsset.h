#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Emerald_MRVR/EKG_Enums.h"
#include "UnitDataAsset.generated.h"

UCLASS()
class EMERALD_MRVR_API UUnitDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
	FName UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
	EKGUnits UnitType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	float Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	int32 Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	bool IsFlyingUnit = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
	UStaticMesh* SM_Unit;

	
};
