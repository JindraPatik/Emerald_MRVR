#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class EMERALD_MRVR_API APowerUp : public AActor
{
	GENERATED_BODY()

public:
	APowerUp();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body")
		TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body")
		TObjectPtr<UStaticMeshComponent> Body;

public:
	virtual void Tick(float DeltaTime) override;
};
