#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Rocket.generated.h"


/* Rocket is a special attacking unit looking for random enemy flying target and trying to instantly destoy it when overlaps */
UCLASS()
class EMERALD_MRVR_API ARocket : public AUnit
{
	GENERATED_BODY()

public:
	ARocket(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY()
		TObjectPtr<AUnit> Target;

	UPROPERTY()
		TObjectPtr<AUnit> PreviousTarget;

	UPROPERTY()
		TArray<AUnit*> ListOfTargets;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
		float MaxSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
		float MaxLaunchSpeed = 50.f;

	UPROPERTY(VisibleAnywhere, Category="Speed")
		float CurrentSpeed = 3.f;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
		float StartAccelerationRate = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
		float AccelerationRate = 2.f;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
		float LaunchTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
		float RotationSpeed = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Life")
		float Lifetime = 20.f;

	bool bHasTargeted = false;

	FVector CurrentDirection;

	UFUNCTION()
		void FindTarget();

	UFUNCTION()
		void SelectTarget();

	UFUNCTION()
		void LaunchRocket(float DeltaTime);

	UFUNCTION()
		void RocketStarted();

	UFUNCTION()
		void MoveToTarget(float DeltaTime);

	UFUNCTION()
		void SearchTargets();

	UFUNCTION()
		void FindAndSelectTarget();

	UFUNCTION()
		void DestroyMe();

	UFUNCTION()
		void Missed();
	
	UFUNCTION()
		void KillTarget(AActor* TargetActor);
	
	UPROPERTY()
		bool bRocketHasLaunched = false;

	UPROPERTY()
		FTimerHandle LaunchHandle;

	UPROPERTY()
		FTimerHandle SearchHandle;

	UPROPERTY()
		FTimerHandle MissedHandle;

	UPROPERTY()
		FTimerHandle DestroyHandle;
	
	FVector CurrentVelocity;
	bool bLaunched = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket")
		float DirectionChangeSpeed = 1.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket")
		float Gravity = 9.80f; 

public:
	virtual void Tick(float DeltaTime) override;

};
