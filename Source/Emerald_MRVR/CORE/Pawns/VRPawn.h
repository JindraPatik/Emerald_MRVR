#pragma once

#include "CoreMinimal.h"
#include "PrimitivePawn.h"
#include "Emerald_MRVR/CORE/GameModes/GameModeMultiplayer.h"
#include "GameFramework/Pawn.h"
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
class EMERALD_MRVR_API AVRPawn : public APrimitivePawn
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
	
	void PerformSphereTrace(
		UMotionControllerComponent* Controller,
		UStaticMeshComponent* ImpactPointer,
		ABuilding*& CurrentlyHoveredBuilding);

	void TogglePlayerInputEnabled();

	UPROPERTY(Replicated)
		bool bInputIsEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
		TSubclassOf<AActor> EndGameWidgetActor;

	UFUNCTION(BlueprintCallable, Category="Player movement")
		void MovePlayerOnCircle(AActor* Player, float InDelta, float& Angle, float Speed);

	UFUNCTION(BlueprintCallable, Category="Player movement")
		void RotatePlayerWithHandGesture(const UMotionControllerComponent* MotionController);

	UFUNCTION()
		void StartGame();

	UFUNCTION()
		void EndGame(APawn* Looser);
public:
	
	// CORE
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CORE")
		AGameModeMultiplayer* GameMode;
	// ~CORE

	// MILITARY BASE
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		UMilitaryStationComp* MilitaryStationComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MilitaryBase")
		TSubclassOf<AActor> PreviewUnitClass;

	UPROPERTY()
		AActor* PreviewInstance;

	UFUNCTION()
		void SpawnPreviewUnit(ABuilding* BuildingActor);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UResourcesComponent> ResourcesComponent;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Base")
		AMilitaryStation* MilitaryStationInstance;	

	bool bPossesed = false;

	UPROPERTY(ReplicatedUsing=OnSelectedModuleChanged, VisibleAnywhere, BlueprintReadWrite, Category="MilitaryBase")
		ABuilding* SelectedBuildingActor = nullptr;

	UFUNCTION()
	void OnSelectedModuleChanged();
};

