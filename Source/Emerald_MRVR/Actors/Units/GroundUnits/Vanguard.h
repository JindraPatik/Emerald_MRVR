#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Vanguard.generated.h"

UCLASS()
class EMERALD_MRVR_API AVanguard : public AUnit
{
	GENERATED_BODY()

public:
	AVanguard();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
