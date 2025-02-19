#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Emerald_MRVR/Actors/MilitaryBase/Building.h"
#include "Emerald_MRVR/Actors/MilitaryBase/MilitaryStation.h"
#include "MilitaryStationComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitSpawnedSignature, AUnit*, Unit, AActor*, Owner);

class UResourcesComponent;
class AMultiplayer_GameMode;
class AUnitAIController;
class ABuilding;
class UUnitDataAsset;
class UBuildingDataAsset;
class AVRPawn;
class AMilitaryStation;
class AUnit;
class ATargetPoint;
class UWidgetAnimation;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UMilitaryStationComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMilitaryStationComp();

private:
	UPROPERTY()
		TObjectPtr<APawn> OwningPawn;				//pb: staci Owner, pripadne Parent nebo OwningPawn

	UPROPERTY()
		TObjectPtr<UResourcesComponent> ResourcesComponentInst;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetSpawnPointForStation();
	void SpawnNotEnoughResWidget();

	UFUNCTION(Server, Reliable)
	void Server_SetSpawnPointForBase();

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
		TSubclassOf<AMilitaryStation> MilitaryStation;

	UPROPERTY(Replicated, BlueprintReadOnly)
		ATargetPoint* SpawnPointForMilitaryStation;

	UPROPERTY(Replicated)
		AMilitaryStation* PlayerMilitaryStationInstance;		//pb: nedoporucuju nazvy typu My..., krom toho slovo Base se obvykle pouziva pro bazove tridy, takze toto na prvni pohled vypada, jako instance nejake bazovew tridy, 
											//    coz je nesmysl

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Units")
		TSubclassOf<AUnit> UnitToSpawn;
	
	UFUNCTION(Category="SpawnBase")
		void SpawnMilitaryStation(APawn* InPawn);				//pb: pozor na konzistenci nazvu, zde OwningPawn, nize InOwner
	
	UFUNCTION(Server, Reliable, Category="SpawnBase")
		void Server_SpawnMilitaryStation(APawn* InPawn);

	UFUNCTION(Category="SpawnBase")
		void SpawnBuildings(APawn* InPawn);
	
	UFUNCTION(Server, Reliable, Category="SpawnBase")
		void Server_SpawnBuildings(APawn* InPawn);

	UFUNCTION(BlueprintCallable, Category="Spawning")
		AUnit* SpawnUnit(APawn* InstigatorPawn, ABuilding* Building);

	UFUNCTION(Server, Reliable, Category="UnitSpawning")
		void Server_SpawnUnit(APawn* InstigatorPawn, ABuilding* Building);

	UFUNCTION(Category="UnitSpawning")
		bool HasEnoughResources(UBuildingDataAsset* BuildingDataAsset);

	UFUNCTION(Server, Reliable)
		void Server_HasEnoughResources(UBuildingDataAsset* BuildingDataAsset);

	UPROPERTY(Replicated, EditAnywhere,Category="UnitSpawning")
		TArray<UBuildingDataAsset*> AvailableBuildings;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category="UnitSpawning")
		TArray<ABuilding*> AvailableBuildingsActors;

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
		FORCEINLINE AMilitaryStation* GetMilitaryBaseInstance() const { return PlayerMilitaryStationInstance; }

	TSubclassOf<AUnitAIController> AiUnitControllerClass;

	UPROPERTY(BlueprintReadWrite, Category="Spawning")
		bool bIsReversed;
};
