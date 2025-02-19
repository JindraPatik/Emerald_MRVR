#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Emerald_MRVR/Actors/MilitaryBase/MilitaryBase.h"
#include "Emerald_MRVR/Actors/MilitaryBase/Building.h"
#include "MilitaryBaseComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitSpawnedSignature, AUnit*, Unit, AActor*, Owner);

class UResourcesComponent;
class AMultiplayer_GameMode;
class AUnitAIController;
class ABuilding;
class UUnitDataAsset;
class UBuildingDataAsset;
class AVRPawn;
class AMilitaryBase;
class AUnit;
class ATargetPoint;
class UWidgetAnimation;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UMilitaryBaseComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMilitaryBaseComp();

private:
	UPROPERTY()
		TObjectPtr<APawn> MyOwner;				//pb: staci Owner, pripadne Parent nebo OwningPawn

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

	UPROPERTY(VisibleInstanceOnly, Category="Widgets")
		TObjectPtr<AActor> NotEnoughResInstance;

	UPROPERTY(EditDefaultsOnly, Category="Sounds")
		TObjectPtr<USoundBase> NotEnoughResSound;
	
public:	
	UPROPERTY(EditDefaultsOnly, Category = "Body")
		TSubclassOf<AMilitaryBase> MilitaryBase;

	UPROPERTY(Replicated, BlueprintReadOnly)
		ATargetPoint* SpawnPointForMilitaryBase;

	UPROPERTY(Replicated)
		AMilitaryBase* MyBaseInstance;		//pb: nedoporucuju nazvy typu My..., krom toho slovo Base se obvykle pouziva pro bazove tridy, takze toto na prvni pohled vypada, jako instance nejake bazovew tridy, 
											//    coz je nesmysl

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Units")
		TSubclassOf<AUnit> UnitToSpawn;
	
	UFUNCTION(Category="SpawnBase")
		void SpawnMilitaryBase(APawn* OwningPawn);				//pb: pozor na konzistenci nazvu, zde OwningPawn, nize InOwner
	
	UFUNCTION(Server, Reliable, Category="SpawnBase")
		void Server_SpawnMilitaryBase(APawn* InOwner);

	UFUNCTION(Category="SpawnBase")
		void SpawnModules(APawn* OwningPawn);
	
	UFUNCTION(Server, Reliable, Category="SpawnBase")
		void Server_SpawnModule(APawn* OwningPawn);

	UFUNCTION(BlueprintCallable, Category="Spawning")
		AUnit* SpawnUnit(APawn* InstigatorPawn, ABuilding* Module);

	UFUNCTION(Server, Reliable, Category="UnitSpawning")
		void Server_SpawnUnit(APawn* InstigatorPawn, ABuilding* Module);

	UFUNCTION(Category="UnitSpawning")
		bool HasEnoughResources(UBuildingDataAsset* BuildingDataAsset);

	UFUNCTION(Server, Reliable)
		void Server_HasEnoughResources(UBuildingDataAsset* BuildingDataAsset);

	UPROPERTY(Replicated, EditAnywhere,Category="UnitSpawning")
		TArray<UBuildingDataAsset*> AvailableModules;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category="UnitSpawning")
		TArray<ABuilding*> AvailableModulesActors;

	UPROPERTY()
		UUnitDataAsset* SelectedUnit;

	UPROPERTY(Replicated)
		FVector UnitSpawnLocation = FVector::ZeroVector;

	UPROPERTY(Replicated)
		FRotator UnitSpawnRotation = FRotator::ZeroRotator;

	UPROPERTY(Replicated)
		AUnit* UnitInstance;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnUnitSpawnedSignature OnUnitSpawnedDelegate;

	UFUNCTION()
		FORCEINLINE AMilitaryBase* GetBaseInstance() const { return MyBaseInstance; }

	TSubclassOf<AUnitAIController> AiUnitControllerClass;

	UPROPERTY(BlueprintReadWrite, Category="Spawning")
		bool bIsReversed;
};
