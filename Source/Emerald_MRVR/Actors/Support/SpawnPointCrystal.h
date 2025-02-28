#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPointCrystal.generated.h"

class UArrowComponent;

UCLASS()
class EMERALD_MRVR_API ASpawnPointCrystal : public AActor
{
	GENERATED_BODY()

public:
	ASpawnPointCrystal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Visuals")
		TObjectPtr<USceneComponent> SpawnRoot;

	UPROPERTY(EditAnywhere, Category="Visuals")
		TObjectPtr<UBillboardComponent> BillboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Visuals")
		TObjectPtr<UArrowComponent> Arrow;

public:

	/* If spawning incorrectly, switch Indexes in the world */
	UPROPERTY(EditInstanceOnly, Category="Mechanics")
		uint8 index = 0;
};
