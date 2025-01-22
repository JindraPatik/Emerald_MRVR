#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

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

public:	
	virtual void Tick(float DeltaTime) override;

	
	
};
