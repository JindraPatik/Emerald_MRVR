#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/ModuleActor.h"
#include "MilitaryBaseComp.generated.h"

class UResourcesComponent;
class AEK_GameMode;
class AUnitAIController;
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

private:
	UPROPERTY()
		TObjectPtr<APawn> MyOwner;

	UPROPERTY()
		TObjectPtr<UResourcesComponent> ResourcesComponentInst;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetSpawnPointForBase();

	UFUNCTION(Server, Reliable)
	void Server_SetSpawnPointForBase();
	void SpawnNotEnoughResWidget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widgets")
		TSubclassOf<AActor> NotEnoughResourcesWidgetActor;

	UPROPERTY(BlueprintReadWrite, Category="Widgets")
		TObjectPtr<AActor>  NotEnoughResourcesWidgetInst;
	
public:	
	UPROPERTY(EditDefaultsOnly, Category = "Body")
		TSubclassOf<AMilitaryBase> MilitaryBase;

	UPROPERTY(Replicated, BlueprintReadOnly)
		ATargetPoint* SpawnPointForMilitaryBase;

	UPROPERTY(Replicated)
		AMilitaryBase* MyBaseInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Units")
		TSubclassOf<AUnit> UnitToSpawn;
	
	UFUNCTION(Category="SpawnBase")
		void SpawnMilitaryBase(APawn* OwningPawn);
	
	UFUNCTION(Server, Reliable, Category="SpawnBase")
		void Server_SpawnMilitaryBase(APawn* InOwner);

	UFUNCTION(Category="SpawnBase")
		void SpawnModules(APawn* OwningPawn);
	
	UFUNCTION(Server, Reliable, Category="SpawnBase")
		void Server_SpawnModule(APawn* OwningPawn);

	UFUNCTION()
		void SpawnUnit(APawn* InstigatorPawn, AModuleActor* Module);

	UFUNCTION(Server, Reliable, Category="UnitSpawning")
		void Server_SpawnUnit(APawn* InstigatorPawn, AModuleActor* Module);

	UFUNCTION(Category="UnitSpawning")
		bool HasEnoughResources(UBuildingDataAsset* BuildingDataAsset) const;

	UFUNCTION(Server, Reliable)
		void Server_HasEnoughResources(UBuildingDataAsset* BuildingDataAsset) const;

	UPROPERTY(Replicated, EditAnywhere, Category="UnitSpawning")
		TArray<UBuildingDataAsset*> AvailableModules;

	UPROPERTY()
		UUnitDataAsset* SelectedUnit;

	UPROPERTY(Replicated)
		FVector UnitSpawnLocation = FVector::ZeroVector;

	UPROPERTY(Replicated)
		FRotator UnitSpawnRotation = FRotator::ZeroRotator;

	UFUNCTION()
		FORCEINLINE AMilitaryBase* GetBaseInstance() const { return MyBaseInstance; }

	TSubclassOf<AUnitAIController> AiUnitControllerClass;

	UPROPERTY()
		AUnitAIController* AIUnitController;
};
