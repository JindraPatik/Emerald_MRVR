#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

class UDownScaleComponent;

UCLASS()
class EMERALD_MRVR_API APowerUp : public AActor
{
	GENERATED_BODY()

public:
	APowerUp();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
		TObjectPtr<UDownScaleComponent> DownScaleComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
		TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
		TObjectPtr<UStaticMeshComponent> Body;
	

public:
	virtual void Tick(float DeltaTime) override;

	/* Implement functionality of PowerUps in Children */
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
		virtual void Activate();

	UFUNCTION(BlueprintCallable, Category = "Overlapping")
		void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
