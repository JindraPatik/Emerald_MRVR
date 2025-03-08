#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Striker.generated.h"

UCLASS()
class EMERALD_MRVR_API AStriker : public AUnit
{
	GENERATED_BODY()

public:
	AStriker(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
