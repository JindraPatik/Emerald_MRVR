#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "PowerUpPicker.generated.h"

/* PowerUpPicker is weak ground unit used for pickng and delivering PowerUps in the world */
UCLASS()
class EMERALD_MRVR_API APowerUpPicker : public AUnit
{
	GENERATED_BODY()

public:
	APowerUpPicker();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
