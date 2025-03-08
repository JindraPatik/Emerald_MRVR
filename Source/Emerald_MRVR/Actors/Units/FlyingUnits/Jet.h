#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Jet.generated.h"

UCLASS()
class EMERALD_MRVR_API AJet : public AUnit
{
	GENERATED_BODY()

public:
	AJet(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
