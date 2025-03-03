#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

UCLASS()
class EMERALD_MRVR_API APowerUp : public AActor
{
	GENERATED_BODY()

public:
	APowerUp();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
		TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Body")
		TObjectPtr<UStaticMeshComponent> Body;

	UFUNCTION()
		void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);

	

public:
	virtual void Tick(float DeltaTime) override;
};
