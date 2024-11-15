#pragma once

#include "CoreMinimal.h"
#include "EK_GameMode.h"
#include "GameFramework/Pawn.h"
#include "MR_General.generated.h"

class APC_MR_General;
class UResourcesComponent;
class UHealthComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CORE")
	AEK_GameMode* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CORE")
	APC_MR_General* PC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Body")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
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

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TSubclassOf<AMilitaryBase> MilitaryBase;

	UPROPERTY(BlueprintReadOnly)
	ATargetPoint* TargetPoint;

	UPROPERTY(ReplicatedUsing=OnRepPosition)
	FVector ReplicatedPosition;

	UPROPERTY(ReplicatedUsing=OnRepRotation)
	FRotator ReplicatedRotation;

	UFUNCTION()
	void OnRepPosition () const {RootComponent->SetWorldLocation(ReplicatedPosition);}

	UFUNCTION()
	void OnRepRotation() const {RootComponent->SetWorldRotation(ReplicatedRotation);}

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Unreliable, Category="Position")
	void Server_UpdatePawnPosition(const FVector& NewPosition, const FRotator& NewRotation);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnMilitaryBase(TSubclassOf<AMilitaryBase> Base);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UResourcesComponent> ResourcesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="spawning")
	FTransform SpawnPoint;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Base")
	AMilitaryBase* BaseInstance;
	
	UFUNCTION(BlueprintCallable, Category="Base")
	FORCEINLINE AMilitaryBase* GetBaseInstance() const { return BaseInstance; }

	
};

