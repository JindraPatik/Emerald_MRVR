
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MR_General.generated.h"

class AMilitaryBase;
class UOculusXRControllerComponent;
class UCameraComponent;
class UMotionControllerComponent;


UCLASS()
class EMERALD_MRVR_API AMR_General : public APawn
{
	GENERATED_BODY()

public:
	AMR_General();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TObjectPtr<UStaticMesh> SM_Body;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TObjectPtr<USceneComponent> Hands;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TObjectPtr<UMotionControllerComponent> MotionController_L;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TObjectPtr<UMotionControllerComponent> MotionController_R;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TSubclassOf<AMilitaryBase> MilitaryBase;

	UPROPERTY()
	AMilitaryBase* BaseInstance;

	


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	AActor* SpawnMilitarybase(TSubclassOf<AMilitaryBase> Base, FVector Location, FRotator Rotation);
	
};
