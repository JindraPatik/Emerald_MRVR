#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Crystal.generated.h"

class UDownScaleComponent;
class UBoxComponent;

UCLASS()
class EMERALD_MRVR_API ACrystal : public AActor
{
	GENERATED_BODY()
	
public:	
	ACrystal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Body")
		USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly, Category="Body")
		UStaticMeshComponent* CrystalBody;	

	UPROPERTY(EditDefaultsOnly, Category="Body")
		UStaticMeshComponent* CrystalBottom;
	
	UPROPERTY(EditDefaultsOnly, Category="Body")
		UBoxComponent* CrystalBox;

	UPROPERTY(EditDefaultsOnly, Category="Scale")
		UDownScaleComponent* DownScaleComponent;

	/* Snap Crystal to ground after spawn */
	UFUNCTION()
		void AdjustLocationAfterSpawn();

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
		float CrystalValue;

	
	
	
};
