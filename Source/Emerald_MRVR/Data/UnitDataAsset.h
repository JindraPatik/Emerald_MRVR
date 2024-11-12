#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnitDataAsset.generated.h"

UCLASS()
class EMERALD_MRVR_API UUnitDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Info")
	FName UnitName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawning")
	float Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	int32 Strength;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visuals")
	TObjectPtr<UStaticMesh> SM_Body;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SFX")
	TObjectPtr<USoundCue> SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SFX")
	TObjectPtr<USoundCue> DeathSound;


	

		
	
};
