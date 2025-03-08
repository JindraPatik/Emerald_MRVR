#pragma once

#include "CoreMinimal.h"
#include "PowerUp.h"
#include "PowerAddResources.generated.h"

UCLASS()
class EMERALD_MRVR_API APowerAddResources : public APowerUp
{
	GENERATED_BODY()

public:
	APowerAddResources(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Activate() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PowerUps")
		float ResourcesToAdd = 50.f;

public:
};
