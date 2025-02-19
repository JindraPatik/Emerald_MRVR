#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Harvester.generated.h"

/* Harvester is Unit for gathering resources in the World.
 * When overlap with Resources object, collect it and returns back to Military station to deliver resources */
UCLASS()
class EMERALD_MRVR_API AHarvester : public AUnit
{
	GENERATED_BODY()

public:
	AHarvester();
	
	UFUNCTION()
		void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

	/* Collect and destroy resources when Overlapped and is not loaded yet */
	UFUNCTION()
		void CollectCrystal(AActor* HittedActor);

	/* Delivers resources to Military station when is loaded */
	UFUNCTION()
		void DeliverCrystal(AActor* HitActor);

	
public:	
	virtual void Tick(float DeltaTime) override;

	bool bIsLoaded = false;
	float HarvestedValue = 0.0f;

};
