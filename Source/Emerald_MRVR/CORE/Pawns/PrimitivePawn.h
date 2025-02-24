#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "PrimitivePawn.generated.h"

class USplineMeshComponent;
class UCapsuleComponent;
class UWidgetInteractionComponent;
class UMotionControllerComponent;
class UCameraComponent;
class UInputMappingContext;
class UMotionControllerComponent;

UCLASS()
class EMERALD_MRVR_API APrimitivePawn : public APawn
{
	GENERATED_BODY()

public:
	APrimitivePawn();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnMousePressed();
	void OnMouseReleased();
	
	
	UFUNCTION(BlueprintCallable)
		void SetUpPointer(UMotionControllerComponent* MotionControllerComponent, float Distance, UStaticMeshComponent* ImpactPointer, UWidgetInteractionComponent* WidgetInteractionComponent, EControllerHand Hand, FHitResult& HitResult);

	// Character Body
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Body")
		TObjectPtr<UCapsuleComponent> VR_Root;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		TObjectPtr<USceneComponent> VR_Proxy;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Body")
		TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly, Category = "Body")
		TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		TObjectPtr<UMotionControllerComponent> MotionController_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		TObjectPtr<UMotionControllerComponent> MotionController_R;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
		TObjectPtr<UStaticMeshComponent> ImpactPointer_L;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
		TObjectPtr<UStaticMeshComponent> ImpactPointer_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
		TObjectPtr<UWidgetInteractionComponent> WidgetInteraction_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
		TObjectPtr<UWidgetInteractionComponent> WidgetInteraction_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
		TObjectPtr<UStaticMeshComponent> PointerStick_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
		TObjectPtr<UStaticMeshComponent> PointerStick_R;

	UPROPERTY(EditDefaultsOnly, Category="Controller")
		float PointerDistance;

	UPROPERTY(BlueprintReadOnly, Category="Controller")
		FHitResult HitResultLeft;
	
	UPROPERTY(BlueprintReadOnly, Category="Controller")
		FHitResult HitResultRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
		UInputMappingContext* MenuInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controller")
		bool bIsMenuActive = false;

	//INPUT ACTIONS
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* Click;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SplineMeshComp")
		USplineMeshComponent* SplineMeshComponent;

public:	
	
	
};
