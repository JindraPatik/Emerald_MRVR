#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Picker.generated.h"

class APowerUp;
/* Picker is weak ground unit used for pickng and delivering PowerUps in the world */
UCLASS()
class EMERALD_MRVR_API APicker : public AUnit
{
	GENERATED_BODY()

public:
	APicker(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		TObjectPtr<APowerUp> PickedPowerUp;

	UPROPERTY()
		bool bIsLoaded = false;

	UFUNCTION()
		void PickPowerUp(APowerUp* InPowerUp, APawn* InOwner);


public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

};
