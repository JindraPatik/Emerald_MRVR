
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MilitaryBase.generated.h"

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Modules")
	TObjectPtr<UBuildingsModuleComponent> BuildingsModuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	USceneComponent* SpawnPoint_Ground;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawning")
	USceneComponent* SpawnPoint_Air;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE FTransform GetSpawnpointGround() const { return SpawnPoint_Ground->GetComponentTransform(); }
	
	FORCEINLINE FTransform GetSpawnpointAir() const { return SpawnPoint_Air->GetComponentTransform();}

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Spawning")
	void Server_SpawnUnit(TSubclassOf<AUnit> UnitToSpawn);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Spawning")
	void Multi_SpawnUnit(TSubclassOf<AUnit> UnitToSpawn);
	

	
	
};
