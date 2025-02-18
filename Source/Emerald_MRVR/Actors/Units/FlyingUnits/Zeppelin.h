#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Zeppelin.generated.h"

UCLASS()
class EMERALD_MRVR_API AZeppelin : public AUnit
{
	GENERATED_BODY()

public:
	AZeppelin();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
