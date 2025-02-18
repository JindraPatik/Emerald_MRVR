#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Tank.generated.h"

UCLASS()
class EMERALD_MRVR_API ATank : public AUnit
{
	GENERATED_BODY()

public:
	ATank();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
