#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Emerald_MRVR/CORE/GameModes/Multiplayer_GameMode.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "VRPawn.generated.h"

class UBuildingsComponent;
class UWidgetInteractionComponent;
class UBuildingDataAsset;
class UMilitaryBaseComp;
class AUnit;
class AVRPlayerController;
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
class ABuilding;
class AActor;

UCLASS()
class EMERALD_MRVR_API AVRPawn : public ABasePawn			//pb: asi bych se klonil k jinemu nazvu nez General, ktery se pouziva v jinem smyslu
{
	GENERATED_BODY()

public:

protected:
	AVRPawn();
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	void SelectBuilding_L();
	void SelectBuilding_R();

	UFUNCTION(BlueprintCallable, Category="Spawning")
		void Action_SpawnUnit();
	
	void PerformSphereTrace(
		UMotionControllerComponent* Controller,
		UStaticMeshComponent* ImpactPointer,
		ABuilding*& CurrentlyHoveredBuilding);
	void EnablePlayerInput();

	UPROPERTY(Replicated)
		bool bInputIsEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
		TSubclassOf<AActor> EndGameWidgetActor;

	UFUNCTION(BlueprintCallable, Category="Player movement")
		void MovePlayerOnCircle(AActor* Player, float InDelta, float& Angle, float Speed);

	UFUNCTION()
		void StartGame();

	UFUNCTION()
		void EndGame(APawn* Looser);

	UPROPERTY()
	ABuilding* PrevisouslyHighlitedBuilding;
	
public:
	
	// CORE
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CORE")
		AMultiplayer_GameMode* GameMode;
	// ~CORE

	// INPUT
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
		UInputMappingContext* GameplayInputMappingContext;
	// ~INPUT


	// MILITARY BASE
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		UMilitaryBaseComp* MilitaryBaseComp;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryBase")
		ABuilding* CurrentlyHoveredBuilding_L;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryBase")
		ABuilding* CurrentlyHoveredBuilding_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryBase")
		TSubclassOf<AActor> PreviewUnitClass;

	UPROPERTY()
		AActor* PreviewInstance;

	UFUNCTION()
		void SpawnPreviewUnit(ABuilding* BuildingActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* IA_SpawnUnit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* IA_SelectBuilding_L;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* IA_SelectBuilding_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* IA_RotatePlayer;

	// Visuals
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
		UMaterialInterface* PlayerDefaultColor;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UResourcesComponent> ResourcesComponent;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Base")
		AMilitaryBase* MilitaryBaseInstance;					//pb: viz. commenty ke konzistenci nazvu

	UPROPERTY(BlueprintReadOnly, Category="CORE")
	bool bGameInitialized = false;

	UPROPERTY(BlueprintReadWrite, Category="CORE")
	bool bIsNotGameplay = false;

	bool bPossesed = false;

	UPROPERTY(ReplicatedUsing=OnSelectedModuleChanged, VisibleAnywhere, BlueprintReadWrite, Category="MilitaryBase")
		ABuilding* SelectedBuildingActor = nullptr;

	UFUNCTION()
	void OnSelectedModuleChanged();
};

