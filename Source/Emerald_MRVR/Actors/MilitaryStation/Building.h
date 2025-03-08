#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"


struct FIsdkInteractionPointerEvent;
class UIsdkPointableBox;
class UIsdkInteractableComponent;

namespace isdk::api
{
	class RayInteractable;
}

class UDownScaleComponent;
class UBuildingDataAsset;
class AVRPawn;
class ACooldownActor;
class UIsdkRayInteractable;
class UIsdkPointableBox;

UCLASS()
class EMERALD_MRVR_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuilding(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Building")
		float IsdkPointableBoxMultiplier = 2.5f;

public:
	void DisableInfoWidget();
	void EnableInfoWidget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
		TSubclassOf<ACooldownActor> CooldownWidgetClass; // priradit v Editoru!

	UPROPERTY()
		TObjectPtr<ACooldownActor> CooldownWidgetInstance;

	UFUNCTION()
		void SpawnCooldownWidget();

	UPROPERTY(BlueprintReadOnly, Category="Pawn")
		TObjectPtr<AVRPawn> VRPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		TObjectPtr<USceneComponent> UnitReturnPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
		TObjectPtr<UDownScaleComponent> DownScaleComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
		TObjectPtr<UIsdkRayInteractable> IsdkInteractable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
		TObjectPtr<UIsdkPointableBox> IsdkPointableBox;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<UStaticMeshComponent> BuildingMeshRoot;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Data")
		TObjectPtr<UBuildingDataAsset> BuildingDataAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
		TSubclassOf<AActor> InfoWidgetActor;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
		TObjectPtr<USceneComponent> InfoWidgetSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visuals")
		TObjectPtr<UMaterialInterface> OverlayMaterial;

	UFUNCTION(BlueprintCallable, Category="Visuals")
		void SetOverlayMaterial();

	UFUNCTION(BlueprintCallable, Category="Visuals")
		void RemoveOverlayMaterial();

	UPROPERTY()
		AActor* InfoWidgetActorInst;
	
	UPROPERTY()
		bool bSpawningEnabled = true;

	UFUNCTION()
		void EnableSpawning();
	
	UFUNCTION()
		void Cooldown(float CD_Time);
	
	UFUNCTION()
		void SpawnInfoWidget();

	FTimerHandle CD_Handle;

	UFUNCTION()
		void SetInfoWidgetStats(AActor* WidgetActor);

	UPROPERTY(EditAnywhere, Category="Widgets")
		float InfoWidgetHeight = 25.f;

	UPROPERTY(EditAnywhere, Category="Widgets")
		float CooldownWidgetHeight = 15.f;

	UFUNCTION(Blueprintpure, Category="Size")
		FVector GetActorBoundingBoxExtent() const;

	UFUNCTION()
	void HandlePointerEvent(const FIsdkInteractionPointerEvent& PointerEvent);
	
	float CooldownDuration;
	float ElapsedTime;
};
