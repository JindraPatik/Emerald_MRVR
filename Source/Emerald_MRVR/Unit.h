#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Unit.generated.h"

class AAIController;
class AUnitAIController;
class UCombatComponent;
class UDownScaleComponent;
class UBoxComponent;
class USphereComponent;
class UUnitMovementComponent;

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
	
public:
	UPROPERTY()
		TObjectPtr<UUnitMovementComponent> UnitMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
		TObjectPtr<UDownScaleComponent> DownScaleComponent;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly, Category="Visuals")
		TObjectPtr<USceneComponent> UnitRoot;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly, Category="Visuals")
		TObjectPtr<UStaticMeshComponent> Body;

	//pb: tyhle data jsou zaroven soucasti UUnitDataAsset. Bylo by dobre to sjednotit - mit pouze jednu strukturu a tu pak vyuzit na vsech mistech, kde je treba
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Stats")
		float Speed;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
		float Strenght;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
		float Price;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Stats")
		float Damage;

	UPROPERTY(BlueprintReadWrite, Category="Stats")
		float FightDelay;

	UPROPERTY(BlueprintReadWrite, Category="Stats")
		bool bIsFlyingUnit;

	UPROPERTY(BlueprintReadWrite, Category="Stats")
		bool bIsAttacker;

	UFUNCTION(BlueprintNativeEvent, Category="Combat")
		void KillMe();

	UPROPERTY(BlueprintReadWrite, Category="Spawning")
		bool bIsReversed;

	UPROPERTY(BlueprintReadWrite, Category="Spawning")
		FVector UnitSpawnLocation;
};
