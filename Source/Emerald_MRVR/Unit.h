#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Unit.generated.h"

class AAIController;
class AUnitAIController;
class UCombatComponent;
class UDownScaleComponent;
class UBoxComponent;
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
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	UDownScaleComponent* DownScaleComponent;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly, Category="Visuals")
	TObjectPtr<USceneComponent> UnitRoot;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	TObjectPtr<UUnitMovementComponent> UnitMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	TObjectPtr<UCombatComponent> CombatComponent;
	
	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly, Category="Visuals")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
	float Speed;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
	float Strenght;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
	float Price;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
	float Damage;


	
};
