#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Digger.generated.h"

/* Digger is Unit for gathering resources in the World.
 * When overlap with Resources object, collect it and returns back to Military base to deliver resources */
UCLASS()
class EMERALD_MRVR_API ADigger : public AUnit
{
	GENERATED_BODY()

public:
	ADigger();
	
	UFUNCTION()
		void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

	/* Collect and destroy resources when Overlapped and is not loaded yet */
	UFUNCTION()
		void CollectCrystal(AActor* HittedActor);

	/* Delivers resources to Military base when is loaded */
	UFUNCTION()
		void DeliverCrystal(AActor* HitActor);

	
public:	
	virtual void Tick(float DeltaTime) override;

	bool bIsLoaded = false;
	float DiggedValue = 0.0f;

};
