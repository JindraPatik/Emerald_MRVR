#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathPoint.generated.h"

UCLASS()
class EMERALD_MRVR_API APathPoint : public AActor
{
	GENERATED_BODY()

public:
	APathPoint();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="index")
	int32 PathIndex = 0;
};
