#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Rebel.generated.h"

/* Rebel unit converts Enemy unit to Player unit when overlapped */
UCLASS()
class EMERALD_MRVR_API ARebel : public AUnit
{
	GENERATED_BODY()

public:
	ARebel(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

public:
	virtual void Tick(float DeltaTime) override;
};
