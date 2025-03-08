#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Helicopter.generated.h"

UCLASS()
class EMERALD_MRVR_API AHelicopter : public AUnit
{
	GENERATED_BODY()

public:
	AHelicopter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
