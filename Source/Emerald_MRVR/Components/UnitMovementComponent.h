#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitMovementComponent.generated.h"

class APathPoint;
class USplineComponent;
class AMilitaryBase;
class APC_MR_General;
class AMR_General;
class AUnit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UUnitMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitMovementComponent();

	UPROPERTY()
	TObjectPtr<AUnit> Unit;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Movement")
		bool bMovementEnabled = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Movement")
		bool bIsRestartingMovement = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Movement")
		bool bIsReversedMovement = false;

	UPROPERTY(BlueprintReadWrite, Category="Movement")
		TObjectPtr<USplineComponent> MovementSpline;
	
	//TODO
	UFUNCTION(BlueprintCallable, Category="Movement")
		void MoveAlongPath(float DeltaTime);

	//TODO
	UFUNCTION(BlueprintCallable, Category="Movement")
		void CreateMovementPath();

	//TODO
	UFUNCTION(BlueprintCallable, Category="Movement")
		void FindPathPoints();

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		TSubclassOf<APathPoint> PathPointClass;

	UPROPERTY(VisibleAnywhere, Category="Movement")
		TArray<APathPoint*> PathPoints;

	UPROPERTY(VisibleAnywhere, Category="Movement")
		float SplineDistance = 0.f;

	UFUNCTION(BlueprintCallable, Category="Movement")
		void Turn180();

	UFUNCTION(BlueprintCallable, Category="Movement")
		void TurnRandom();

	UFUNCTION(BlueprintCallable, Category="Movement")
		void StopUnit();

	UFUNCTION(BlueprintCallable, Category="Movement")
		void Accelerate(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="Movement")
		void RestartMovement();

	float StartingSpeed = 0;
	bool bIsFlying;
	bool bIsReversed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		float Acceleration = 10.f; // Maybe add to Data asset?

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="Stats")
		float UnitSpeed;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float RollAmountMultiplier = 60.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float SmoothRotationInterpSpeed = 5.f;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
