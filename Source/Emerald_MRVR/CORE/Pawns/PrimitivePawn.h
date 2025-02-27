#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PrimitivePawn.generated.h"

class AEKGameState;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controller")
		bool bIsMenuActive = false;

public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CORE")
		TObjectPtr<AEKGameState> EKGameState;
};
