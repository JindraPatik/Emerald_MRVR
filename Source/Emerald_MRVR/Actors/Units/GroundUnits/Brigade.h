#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Brigade.generated.h"

UCLASS()
class EMERALD_MRVR_API ABrigade : public AUnit
{
	GENERATED_BODY()

public:
	ABrigade();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
