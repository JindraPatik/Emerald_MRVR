#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

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

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<UStaticMeshComponent> BaseBody;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<UDownScaleComponent> DownScaleComponent;


public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		TObjectPtr<UUnitMovementComponent> MovementComponent;

	
	
};
