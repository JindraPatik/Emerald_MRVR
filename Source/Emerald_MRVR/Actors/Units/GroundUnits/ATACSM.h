#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "ATACSM.generated.h"

UCLASS()
class EMERALD_MRVR_API AATACSM : public AUnit
{
	GENERATED_BODY()

public:
	AATACSM();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
