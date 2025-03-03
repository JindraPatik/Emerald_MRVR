#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldWrapper.generated.h"

class UDownScaleComponent;

UCLASS()
class EMERALD_MRVR_API AWorldWrapper : public AActor
{
	GENERATED_BODY()

public:
	AWorldWrapper();

protected:
	virtual void BeginPlay() override;

	TObjectPtr<UDownScaleComponent> DownScaleComponent;

public:
};
