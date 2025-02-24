#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Voyager.generated.h"

UCLASS()
class EMERALD_MRVR_API AVoyager : public AUnit
{
	GENERATED_BODY()

public:
	AVoyager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
