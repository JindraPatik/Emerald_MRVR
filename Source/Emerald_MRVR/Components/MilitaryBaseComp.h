#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MilitaryBaseComp.generated.h"


class AModuleActor;
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

	UPROPERTY()
	AMR_General* General; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="spawning")
	FTransform SpawnPoint; // Military Base Unit spawn point

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TSubclassOf<AMilitaryBase> MilitaryBase;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	TSubclassOf<AModuleActor> ModuleClass;

	UPROPERTY(Replicated, BlueprintReadOnly)
	ATargetPoint* SpawnPointForMilitaryBase;

	UPROPERTY()
	AMilitaryBase* MyBaseInstance;

	UPROPERTY(Replicated)
	FVector SpawnLocation;
	
	UPROPERTY(Replicated)
	FRotator SpawnRotation;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Units")
	TSubclassOf<AUnit> UnitToSpawn;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(Category="SpawnBase")
	void SpawnMilitaryBase(AMR_General* OwningPawn);
	
	UFUNCTION(Server, Reliable, Category="SpawnBase")
	void Server_SpawnMilitaryBase(AMR_General* OwningPawn);

	UFUNCTION(Category="SpawnBase")
	void SpawnModules(AMR_General* OwningPawn);
	
	UFUNCTION(Server, Reliable, Category="SpawnBase")
	void Server_SpawnModule(AMR_General* OwningPawn);

	/*UFUNCTION()
	void SpawnUnit();

	UFUNCTION(Server, Reliable, Category="UnitSpawning")
	void Server_SpawnUnit();

	UFUNCTION(Category="UnitSpawning")
	bool HasEnoughResources() const;*/

	UPROPERTY(Replicated, EditAnywhere, Category="UnitSpawning")
	TArray<UBuildingDataAsset*> AvailableModules;

	UPROPERTY(VisibleAnywhere, Category="UnitSpawning")
	UUnitDataAsset* SelectedUnit;

	void GetMilitaryBaseSpawnPoint();

	UFUNCTION(Server, Reliable)
	void Server_GetMilitaryBaseSpawnPoint();
};
