#pragma once

#include "CoreMinimal.h"
#include "PowerUp.h"
#include "PowerAddHealth.generated.h"

UCLASS()
class EMERALD_MRVR_API APowerAddHealth : public APowerUp
{
	GENERATED_BODY()

public:
	APowerAddHealth();

protected:
	virtual void BeginPlay() override;
	virtual void Activate() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PowerAddHealth")
		float HealthToAdd = 15.f;

public:
};
