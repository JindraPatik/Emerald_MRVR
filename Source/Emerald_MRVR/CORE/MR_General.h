
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MR_General.generated.h"

class UCharacterMovementComponent;
class AMilitaryBase;
class UOculusXRControllerComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UStaticMeshComponent;
class ATargetPoint;

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

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Body")
	TObjectPtr<UStaticMeshComponent> GeneralBody;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TObjectPtr<USceneComponent> Hands;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
	TObjectPtr<UMotionControllerComponent> MotionController_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
	TObjectPtr<UMotionControllerComponent> MotionController_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	UStaticMeshComponent* ImpactPointer_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	UStaticMeshComponent* ImpactPointer_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	UCharacterMovementComponent* CharacterMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TSubclassOf<AMilitaryBase> MilitaryBase;

	UPROPERTY()
	AMilitaryBase* BaseInstance;

	UPROPERTY(BlueprintReadOnly)
	ATargetPoint* TargetPoint;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnMilitaryBase(TSubclassOf<AMilitaryBase> Base);

};
