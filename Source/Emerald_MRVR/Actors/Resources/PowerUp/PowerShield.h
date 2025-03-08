#pragma once

#include "CoreMinimal.h"
#include "PowerUp.h"
#include "PowerShield.generated.h"

class AShieldActor;

UCLASS()
class EMERALD_MRVR_API APowerShield : public APowerUp
{
	GENERATED_BODY()

public:
	APowerShield(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Activate() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shield")
		TSubclassOf<AShieldActor> ShieldActorClass;

public:
	virtual void Tick(float DeltaTime) override;
};
