#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Unit.generated.h"

class USphereComponent;
class UUnitMovementComponent;

UCLASS()
class EMERALD_MRVR_API AUnit : public APawn
{
	GENERATED_BODY()

public:
	AUnit();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Collision")
	TObjectPtr<USphereComponent> SphereComponent;


public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	TObjectPtr<UUnitMovementComponent> UnitMovementComponent;
	
	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly, Category="Visuals")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float Speed;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
	float Strenght;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
	float Price;

	/*UFUNCTION()
	void SetUnitStats(float InSpeed, float InPrice, float InStrenght);*/


	
};
