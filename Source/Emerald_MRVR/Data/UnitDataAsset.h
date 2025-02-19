#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Emerald_MRVR/Support/EmeraldEnums.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
		float FightDelay;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		bool IsFlyingUnit = false;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		bool IsAttacker = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		bool IsRocket = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
		UStaticMesh* UnitBody;

	
};
