#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Thief.generated.h"

/* Thief Unit steals Resources when overlaps with Enemy loaded Harvester.
 * When overlaps with Enemy station steals amount of resources from enemy player.
 * In both cases has to return to Player's station and deliver Resources.
 * When is loaded, has decreased speed.
 */
UCLASS()
class EMERALD_MRVR_API AThief : public AUnit
{
	GENERATED_BODY()

public:
	AThief();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
		void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

	/* Steals resources from loaded Harvester and return to station */
	UFUNCTION()
		void HarvesterRobbery(AActor* OtherActor);

	UFUNCTION()
		void StationRobbery(AActor* OtherActor);

	UPROPERTY(EditAnywhere, Category="Speed")
		float ReturnSlowdownPercent = 20.f;

	float ReturnSpeed;
	
public:	
	UPROPERTY()
		bool bIsloaded = false;

	UPROPERTY()
		float StealedValue = 0;

	UPROPERTY(EditDefaultsOnly, Category="Stealing")
		float AmountToStealFromStation = 10.f;

public:
	virtual void Tick(float DeltaSeconds) override;

};
