#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Projectile.generated.h"

class UBoxComponent;
class UUnitMovementComponent;
class UDownScaleComponent;

UCLASS()
class EMERALD_MRVR_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<UStaticMeshComponent> BaseBody;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<UDownScaleComponent> DownScaleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
			TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		float ProjectileImpulseMultiplier = 1;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		float Speed;
	
	UFUNCTION()
	void OnBoxOverlapped(	UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);
	
};
