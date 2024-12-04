#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MilitaryBaseComp.generated.h"


class UUnitDataAsset;
class UBuildingDataAsset;
class AMR_General;
class AMilitaryBase;
class AUnit;
class ATargetPoint;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UMilitaryBaseComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMilitaryBaseComp();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	AMR_General* General; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BaseBody")
	FTransform BaseBody; // Military Base Unit spawn point
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="spawning")
	FTransform SpawnPoint; // Military Base Unit spawn point

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TSubclassOf<AMilitaryBase> MilitaryBase;

	UPROPERTY(BlueprintReadOnly)
	ATargetPoint* TargetPoint;

	AMilitaryBase* MyBaseInstance;

	UPROPERTY(Replicated, VisibleAnywhere)
	FVector UnitTargetLoc;


	UFUNCTION()
	void SelectUnitToSpawn(UBuildingDataAsset* SelectedBuilding);



public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Units")
	TSubclassOf<AUnit> UnitToSpawn;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(Category="SpawnBase")
	void SpawnMilitaryBase(AMR_General* OwningPawn);
	
	UFUNCTION(Server, Reliable, Category="SpawnBase")
	void Server_SpawnMilitaryBase(AMR_General* OwningPawn);

	UFUNCTION()
	void SpawnUnit();

	UFUNCTION(Server, Reliable, Category="UnitSpawning")
	void Server_SpawnUnit();

	UFUNCTION(Category="UnitSpawning")
	bool HasEnoughResources() const;

	UPROPERTY(VisibleAnywhere, Category="UnitSpawning")
	TArray<UBuildingDataAsset*> AvailableModules;

	UPROPERTY(VisibleAnywhere, Category="UnitSpawning")
	UUnitDataAsset* SelectedUnit;
};
