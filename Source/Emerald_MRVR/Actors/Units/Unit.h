#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Unit.generated.h"

class ABuilding;
class AAIController;
class AUnitAIController;
class UCombatComponent;
class UDownScaleComponent;
class UBoxComponent;
class USphereComponent;
class UUnitMovementComponent;

/* Base class for Units */
UCLASS()
class EMERALD_MRVR_API AUnit : public APawn
{
	GENERATED_BODY()

public:
	AUnit();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* Component for scaling Actor root*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
		TObjectPtr<UDownScaleComponent> DownScaleComponent;

	UFUNCTION()
		void DestroyOnReturn(AActor* OverlappedActor, AActor* OtherActor);
public:
	UPROPERTY()
		TObjectPtr<UUnitMovementComponent> UnitMovementComponent;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Visuals")
		TObjectPtr<USceneComponent> UnitRoot;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly, Category="Visuals")
		TObjectPtr<UStaticMeshComponent> Body;

	//pb: tyhle data jsou zaroven soucasti UUnitDataAsset. Bylo by dobre to sjednotit - mit pouze jednu strukturu a tu pak vyuzit na vsech mistech, kde je treba
	UPROPERTY(Replicated, VisibleAnywhere, Category="Stats")
		float Speed;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Stats")
		float Strenght;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Stats")
		float Price;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category="Stats")
		float Damage;

	UPROPERTY(VisibleAnywhere, Category="Stats")
		float FightDelay;

	UPROPERTY(VisibleAnywhere, Category="Stats")
		bool bIsFlyingUnit;

	UPROPERTY(VisibleAnywhere, Category="Stats")
		bool bIsAttacker;

	UPROPERTY(VisibleAnywhere, Category="Stats")
		bool bIsReversed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
		bool HasReturnPoint = false;

	UPROPERTY(VisibleAnywhere, Category="Spawning")
		FVector UnitSpawnLocation;

	UPROPERTY()
		TObjectPtr<ABuilding> OwningBuilding;

	UPROPERTY()
		bool bHasAttacked = false;

	/* Function to handle Death of the Unit Actor */
	UFUNCTION(BlueprintNativeEvent, Category="Combat")
		void KillMe();

	UFUNCTION()
		void AlreadyAttacked();
};
