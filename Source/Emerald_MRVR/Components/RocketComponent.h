#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RocketComponent.generated.h"


class AUnit;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EMERALD_MRVR_API URocketComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URocketComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		TObjectPtr<AUnit> Target;

	UPROPERTY()
		TObjectPtr<AUnit> PreviousTarget;

	UPROPERTY()
		TArray<AUnit*> ListOfTargets;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
		float MaxSpeed = 100.f;

	UPROPERTY(VisibleAnywhere, Category="Speed")
		float CurrentSpeed = 3.f;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
		float AccelerationRate = 2.f;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
		float LaunchTime = 3.f;

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
		void FindAndSelect();

	UPROPERTY()
		bool bRocketHasLaunched = false;

	UPROPERTY()
		FTimerHandle LaunchHandle;

	UPROPERTY()
		FTimerHandle SearchHandle;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
