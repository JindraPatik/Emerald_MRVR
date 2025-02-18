#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Explorer.generated.h"

UCLASS()
class EMERALD_MRVR_API AExplorer : public AUnit
{
	GENERATED_BODY()

public:
	AExplorer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
