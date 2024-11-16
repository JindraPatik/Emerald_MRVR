#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Unit.generated.h"

class UUnitMovementComponent;

UCLASS()
class EMERALD_MRVR_API AUnit : public APawn
{
	GENERATED_BODY()

public:
	AUnit();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	TObjectPtr<UUnitMovementComponent> UnitMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float Strenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float Price;
	

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
