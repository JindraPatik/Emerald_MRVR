#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Picker.generated.h"

/* Picker is weak ground unit used for pickng and delivering PowerUps in the world */
UCLASS()
class EMERALD_MRVR_API APicker : public AUnit
{
	GENERATED_BODY()

public:
	APicker();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
