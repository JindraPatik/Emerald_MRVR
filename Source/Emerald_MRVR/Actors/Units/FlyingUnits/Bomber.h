#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Bomber.generated.h"

class AProjectile;


/* Flying Unit which scans ground for Enemy units.
 * When valid target is found, drops amount of projectiles in MaxBombInterval */
UCLASS()
class EMERALD_MRVR_API ABomber : public AUnit
{
	GENERATED_BODY()

public:
	ABomber();

protected:
	virtual void BeginPlay() override;

	/* Has unit found Valid target? */
	bool bFoundValidTarget = false;

	/* Pointer to hold reference of valid Target */
	UPROPERTY()
	TObjectPtr<AUnit> Target;

	/* Min time of bombing sequence */
	UPROPERTY(EditDefaultsOnly, Category="Bombing")
	float MinBombInterval = 0.2f;

	/* Max time of bombing sequence */
	UPROPERTY(EditDefaultsOnly, Category="Bombing")
	float MaxBombInterval = 2.f;
	
	/* Radius of Sphere trace sphere */
	UPROPERTY(EditDefaultsOnly, Category="Bombing")
	float ScanningSphereRadius = 5.f;

	/* Counter how many Projectiles has been spawned yet */
	uint32 ProjectilesCount = 0;

	/* How many Projectiles spawn during bombing sequence */
	UPROPERTY(EditDefaultsOnly, Category="Bombing")
	uint32 ProjectilesToSpawn = 3;
	
	/* Select Projectile Class in Editor */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	TSubclassOf<AProjectile> ProjectileClass;

	/* Sphere trace for searching AUnits */
	UFUNCTION()
	void PerformSphereTrace(FHitResult& OutHit) const;

	/* Checks if it is Enemy target */
	UFUNCTION()
	void FindValidTarget(AActor* Unit);

	/* Timer for random spawning Projectiles in desired Interval */
	UFUNCTION()
	void StartBombingSequence();

	/* Projectiles spawning */
	UFUNCTION()
	void SpawnProjectile();

	FTimerHandle BombingSequence;

public:
	virtual void Tick(float DeltaTime) override;
};
