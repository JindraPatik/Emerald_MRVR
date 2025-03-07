#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShieldActor.generated.h"

UCLASS()
class EMERALD_MRVR_API AShieldActor : public AActor
{
	GENERATED_BODY()

public:
	AShieldActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
		TObjectPtr<UStaticMeshComponent> Body;

	UFUNCTION()
		void UpScaleShield(float DeltaTime);
	
	UFUNCTION()
		void DestroyShield();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
		float ShieldMaxRadius = 0.7f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
		float ShieldRadius = 0.f;

	UPROPERTY()
		bool bIsUpscaling = false;


public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);
	
};
