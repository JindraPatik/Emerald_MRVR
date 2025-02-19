#pragma once

#include "CoreMinimal.h"
#include "PrimitivePawn.h"
#include "Emerald_MRVR/CORE/GameModes/Multiplayer_GameMode.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "VRPawn.generated.h"

class UBuildingsComponent;
class UWidgetInteractionComponent;
class UBuildingDataAsset;
class UMilitaryStationComp;
class AUnit;
class AVRPlayerController;
class UResourcesComponent;
class UHealthComponent;
class UCharacterMovementComponent;
class AMilitaryStation;
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
class EMERALD_MRVR_API AVRPawn : public APrimitivePawn			//pb: asi bych se klonil k jinemu nazvu nez General, ktery se pouziva v jinem smyslu
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


	// MILITARY Station
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		UMilitaryStationComp* MilitaryStationComp;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryStation")
		ABuilding* CurrentlyHoveredBuilding_L;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryStation")
		ABuilding* CurrentlyHoveredBuilding_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryStation")
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
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Station")
		AMilitaryStation* MilitaryStationInstance;					//pb: viz. commenty ke konzistenci nazvu

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

