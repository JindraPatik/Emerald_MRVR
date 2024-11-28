#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UMotionControllerComponent;
class UCameraComponent;

UCLASS()
class EMERALD_MRVR_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	void SetUpPointer(UMotionControllerComponent* MotionControllerComponent, float Distance, UStaticMeshComponent* ImpactPointer, UWidgetInteractionComponent* WidgetInteractionComponent, EControllerHand Hand, FHitResult& HitResult);

	// Character Body
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Body")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Body")
	TObjectPtr<UStaticMeshComponent> GeneralBody;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TObjectPtr<USceneComponent> Hands;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
	TObjectPtr<UMotionControllerComponent> MotionController_L;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
	TObjectPtr<UMotionControllerComponent> MotionController_R;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	TObjectPtr<UStaticMeshComponent> ImpactPointer_L;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	TObjectPtr<UStaticMeshComponent> ImpactPointer_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	TObjectPtr<UWidgetInteractionComponent> WidgetInteraction_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	TObjectPtr<UWidgetInteractionComponent> WidgetInteraction_R;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	TObjectPtr<UStaticMeshComponent> PointerStick_L;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	TObjectPtr<UStaticMeshComponent> PointerStick_R;

	UPROPERTY(EditDefaultsOnly, Category="Controller")
	float PointerDistance;

	UPROPERTY(BlueprintReadOnly, Category="Controller")
	FHitResult HitResultLeft;
	
	UPROPERTY(BlueprintReadOnly, Category="Controller")
	FHitResult HitResultRight;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	
	
};
