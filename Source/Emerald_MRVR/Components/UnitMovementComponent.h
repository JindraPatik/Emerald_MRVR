#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitMovementComponent.generated.h"

class AMilitaryBase;
class APC_MR_General;
class AMR_General;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UUnitMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitMovementComponent();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		bool bMovementEnabled = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		bool bIsRestartingMovement = false;
	   
	UFUNCTION(BlueprintCallable, Category="Movement")
		void MoveTo(float DeltaTime) const;

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		float Acceleration = 10.f; // Maybe add to Data asset?

	


	


	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="Stats")
		float UnitSpeed;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		float OscillationFrequency = 2.0f;      // Frekvence oscilací

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		float OscillationAmplitudeX = 0.03f;   // Amplituda pohybu do stran

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		float OscillationAmplitudeZ = 0.005f;   // Amplituda pohybu nahoru/dolů

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		float RollFactor = 75.0f;               // Míra naklonění na základě pohybu do stran

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
		float RollInterpolationSpeed = 5.0f;   // Rychlost změny Roll

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
