#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitMovementComponent.generated.h"

class APathPoint;
class USplineComponent;
class AMilitaryStation;
class AVRPlayerController;
class AVRPawn;
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

	/* Reversed movement has Enemy to move along path in reversed direction */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Movement")
		bool bIsReversedMovement = false;

	UPROPERTY(BlueprintReadWrite, Category="Movement")
		TObjectPtr<USplineComponent> MovementSpline;
	
	UFUNCTION(BlueprintCallable, Category="Movement")
		void MoveAlongPath(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="Movement")
		void CreateMovementPath();

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
		void ExtendMovementPathToReturn(FTransform StartLocation, FTransform EndLocation);

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
		float Acceleration = 15.f; // Maybe add to Data asset?

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="Stats")
		float UnitSpeed;

	UFUNCTION()
		void BeginOvertake();

	UFUNCTION()
		void EndOvertake();

	UFUNCTION()
		void StartAvoidUnit();

	UFUNCTION()
		void EndAvoidUnit();

	

	UPROPERTY()
		bool bIsOvertakingSlower = false;

	UFUNCTION()
		void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float RollAmountMultiplier = 60.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float SmoothRotationInterpSpeed = 5.f;

	UFUNCTION()
		FVector GetTurnPoint(FVector CurrentLocation, FRotator CurrentRotation, float TurnAngle, float Distance);

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float TargetZOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float CurrentZOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float HeightSpeed = 15.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float BaseZ = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float TargetXOffset  = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float CurrentXOffset   = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
		float SideMoveSpeed    = 15.0f;

	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
