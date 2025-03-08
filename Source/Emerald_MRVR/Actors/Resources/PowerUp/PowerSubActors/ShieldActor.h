#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShieldActor.generated.h"

UCLASS()
class EMERALD_MRVR_API AShieldActor : public AActor
{
	GENERATED_BODY()

public:
	AShieldActor(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<UStaticMeshComponent> Body;

	UFUNCTION()
		void UpScaleShield(float DeltaTime);

	UFUNCTION()
		void DownScaleShield(float DeltaTime);

	UFUNCTION()
		void DeactivateShield();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
		float ShieldMinRadius = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
		float ShieldMaxRadius = 0.7f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
		float ShieldRadius = 0.f;

	UPROPERTY()
		bool bIsUpscaling = false;

	UPROPERTY()
		bool bIsDownscaling = false;

	UPROPERTY()
		float ShieldDuration = 10.f;

	UPROPERTY()
		FTimerHandle ShieldDeactivationTimer;

	UPROPERTY()
	float ScalingMultiplicator = 1.5f;


public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);
	
};
