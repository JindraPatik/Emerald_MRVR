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

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Collision")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	TObjectPtr<UUnitMovementComponent> UnitMovementComponent;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float Speed;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
	float Strenght;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
	float Price;

	UFUNCTION()
	void SetUnitStats();


	
};
