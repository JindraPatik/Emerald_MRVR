#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Collaborator.generated.h"

/* Collaborator unit converts Enemy unit to Player unit when overlapped */
UCLASS()
class EMERALD_MRVR_API ACollaborator : public AUnit
{
	GENERATED_BODY()

public:
	ACollaborator();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

public:
	virtual void Tick(float DeltaTime) override;
};
