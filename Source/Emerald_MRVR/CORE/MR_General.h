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

UCLASS()
class EMERALD_MRVR_API AMR_General : public ABasePawn
{
	GENERATED_BODY()

public:
	
	void SetPlayerColor();
	void Action_SpawnUnit();


protected:
	AMR_General();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
public:
	// CORE
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CORE")
	AEK_GameMode* GameMode;

	// ~CORE

	// INPUT
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* GameplayInputMappingContext;


	// Character Movement
	UPROPERTY(ReplicatedUsing=OnRepPosition)
	FVector ReplicatedPosition;

	UPROPERTY(ReplicatedUsing=OnRepRotation)
	FRotator ReplicatedRotation;

	UFUNCTION()
	void OnRepPosition () const {RootComponent->SetWorldLocation(ReplicatedPosition);}

	UFUNCTION()
	void OnRepRotation() const {RootComponent->SetWorldRotation(ReplicatedRotation);}

	UFUNCTION(Server, Unreliable, Category="Position")
	void Server_UpdatePawnPosition(const FVector& NewPosition, const FRotator& NewRotation);

	// ~INPUT


	// MILITARY BASE
	void SpawnMilitaryBase();
	void SelectBuilding();
	
	UPROPERTY(EditDefaultsOnly, Category = "Body")
	UMilitaryBaseComp* MilitaryBaseComp;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="MilitaryBase")
	TArray<UBuildingDataAsset*> AvailableBuildings;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="MilitaryBase")
	UBuildingsModuleComponent* CurrentlySelectedModule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* DebugSpawnUnit;

	// doplnit nebo smazat
	UPROPERTY(VisibleAnywhere, Category="MilitaryBase")
	TMap<FName, int32> BuildingsMap;

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

	UPROPERTY(EditDefaultsOnly, Category="Testing")
	TSubclassOf<AUnit> DefaultUnit;
};

