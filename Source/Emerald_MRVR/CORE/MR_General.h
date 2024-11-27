#pragma once

#include "CoreMinimal.h"
#include "EK_GameMode.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "MR_General.generated.h"

class UBuildingsModuleComponent;
class UWidgetInteractionComponent;
class UBuildingDataAsset;
class UMilitaryBaseComp;
class AUnit;
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
class UInputMappingContext;
class UInputAction;

UCLASS()
class EMERALD_MRVR_API AMR_General : public APawn
{
	GENERATED_BODY()

public:
	AMR_General();
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetPlayerColor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Controller")
	float PointerDistance;
	
	
public:
	// CORE
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CORE")
	AEK_GameMode* GameMode;

	// ~CORE

	// INPUT
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* DebugSpawnUnit;

	UFUNCTION(BlueprintCallable)
	void SetUpPointer(UMotionControllerComponent* MotionControllerComponent, float Distance, UStaticMeshComponent* ImpactPointer, UWidgetInteractionComponent* WidgetInteractionComponent, EControllerHand Hand, FHitResult& HitResult);
	// ~INPUT

	// Character Body
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Body")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TObjectPtr<UStaticMeshComponent> GeneralBody;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	UMaterialInterface* PlayerDefaultColor;

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
	UWidgetInteractionComponent* WidgetInteraction_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Controller")
	UWidgetInteractionComponent* WidgetInteraction_R;

	// MILITARY BASE
	UPROPERTY(EditDefaultsOnly, Category = "Body")
	UMilitaryBaseComp* MilitaryBaseComp;

	UFUNCTION()
	void SpawnMilitaryBase();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="MilitaryBase")
	TArray<UBuildingDataAsset*> AvailableBuildings;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="MilitaryBase")
	UBuildingsModuleComponent* CurrentlySelectedModule;

	// doplnit nebo smazat
	UPROPERTY(VisibleAnywhere, Category="MilitaryBase")
	TMap<FName, int32> BuildingsMap;
	
	UFUNCTION()
	void SelectBuilding();

	// Character Movement
	UPROPERTY(ReplicatedUsing=OnRepPosition)
	FVector ReplicatedPosition;

	UPROPERTY(ReplicatedUsing=OnRepRotation)
	FRotator ReplicatedRotation;

	UFUNCTION()
	void OnRepPosition () const {RootComponent->SetWorldLocation(ReplicatedPosition);}

	UFUNCTION()
	void OnRepRotation() const {RootComponent->SetWorldRotation(ReplicatedRotation);}

	UFUNCTION()
	void Action_SpawnUnit();

public:	
	UFUNCTION(Server, Unreliable, Category="Position")
	void Server_UpdatePawnPosition(const FVector& NewPosition, const FRotator& NewRotation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UResourcesComponent> ResourcesComponent;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Base")
	AMilitaryBase* BaseInstance;

	UPROPERTY(BlueprintReadOnly, Category="Controller")
	FHitResult HitResultLeft;
	
	UPROPERTY(BlueprintReadOnly, Category="Controller")
	FHitResult HitResultRight;

	void DetectModule(FHitResult HitResult);

	UPROPERTY(BlueprintReadOnly, Category="CORE")
	bool bGameInitialized = false; 
};

