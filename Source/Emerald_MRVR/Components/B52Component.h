#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "B52Component.generated.h"


class AProjectile;
class AUnit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UB52Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UB52Component();

protected:
	virtual void BeginPlay() override;

	bool bFoundValidTarget = false;

	UPROPERTY()
		TObjectPtr<AUnit> Target;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TSubclassOf<AProjectile> ProjectileClass;
	
	UFUNCTION()
		void PerformSphereTrace(FHitResult& OutHit);

	UFUNCTION()
		void FindValidTarget(AActor* Unit);

	UFUNCTION()
		void SpawnProjectile();

	


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
