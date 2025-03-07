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
		void DestroyShield();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
		float ShieldRadius = 0.7f;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

	
};
