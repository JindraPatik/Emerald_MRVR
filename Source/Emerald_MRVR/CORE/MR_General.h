#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
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
class AModuleActor;
class AActor;

UCLASS()
class EMERALD_MRVR_API AMR_General : public ABasePawn
{
	GENERATED_BODY()

public:
	
	void SetPlayerColor();

protected:
	AMR_General();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	void PerformSphereTrace(
		TObjectPtr<UMotionControllerComponent> Controller,
		TObjectPtr<UStaticMeshComponent> ImpactPointer,
		AModuleActor*& CurrentlyHoveredModule);
	void SelectModule_L();
	void SelectModule_R();
	void Action_SpawnUnit();

	
public:
	// CORE
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CORE")
		AEK_GameMode* GameMode;

	// ~CORE

	// INPUT
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
		UInputMappingContext* GameplayInputMappingContext;

	// Character Movement
	void UpadatePosition(FVector HMDPosition, FRotator HMDOrientation);

	UFUNCTION(Server, Unreliable)
		void Server_UpdatePawnPosition(FVector HMDPosition, FRotator HMDOrientation);

	// ~INPUT


	// MILITARY BASE
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		UMilitaryBaseComp* MilitaryBaseComp;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryBase")
		AModuleActor* CurrentlyHoveredModule_L;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryBase")
		AModuleActor* CurrentlyHoveredModule_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryBase")
		TSubclassOf<AActor> PreviewUnitClass;

	UPROPERTY()
		AActor* PreviewInstance;

	UFUNCTION()
		void SpawnPreviewUnit(AModuleActor* ModuleActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* IA_SpawnUnit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* IA_SelectModule_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* IA_SelectModule_R;

	// Visuals
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
		UMaterialInterface* PlayerDefaultColor;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UResourcesComponent> ResourcesComponent;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Base")
		AMilitaryBase* BaseInstance;

	UPROPERTY(BlueprintReadOnly, Category="CORE")
	bool bGameInitialized = false;

	UPROPERTY(BlueprintReadWrite, Category="CORE")
	bool bIsNotGameplay = false;

	bool bPossesed = false;

	UPROPERTY(ReplicatedUsing=OnSelectedModuleChanged, VisibleAnywhere, Category="MilitaryBase")
		AModuleActor* SelectedModuleActor = nullptr;

	UFUNCTION()
	void OnSelectedModuleChanged();
};

