
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MilitaryBase.generated.h"

class APC_MR_General;
class AUnit;
class UHealthComponent;
class UBuildingsModuleComponent;
class UDownScaleComponent;
class UBoxComponent;

UCLASS()
class EMERALD_MRVR_API AMilitaryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMilitaryBase();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, Category = "Visuals")
	TObjectPtr<UStaticMeshComponent> BaseBody;

	UPROPERTY(EditAnywhere, Category = "BaseBody")
	TObjectPtr<UBoxComponent> BaseBox;

	UPROPERTY(VisibleDefaultsOnly, Category="Visuals")
	UDownScaleComponent* DownScaleComponent;

	// Building modules
	UPROPERTY(EditAnywhere, Category = "Buildings")
	TObjectPtr<USceneComponent> Modules;

	// Mine
	UPROPERTY(EditAnywhere, Category = "Buildings")
	TObjectPtr<UStaticMeshComponent> Mine;
	
	UPROPERTY(EditAnywhere, Category = "Buildings")
	TObjectPtr<UBoxComponent> MineBox;

	// Mine
	UPROPERTY(EditAnywhere, Category = "Buildings")
	TObjectPtr<UStaticMeshComponent> Barracs;
	
	UPROPERTY(EditAnywhere, Category = "Buildings")
	TObjectPtr<UBoxComponent> BarracsBox;

	// Garage
	UPROPERTY(EditAnywhere, Category = "Buildings")
	TObjectPtr<UStaticMeshComponent> Garage;
	
	UPROPERTY(EditAnywhere, Category = "Buildings")
	TObjectPtr<UBoxComponent> GarageBox;


public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Modules")
	TObjectPtr<UBuildingsModuleComponent> BuildingsModuleComponent;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	USceneComponent* SpawnPoint_Ground;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	USceneComponent* SpawnPoint_Air;
	
};
