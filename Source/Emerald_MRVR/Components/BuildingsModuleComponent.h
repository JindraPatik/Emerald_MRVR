#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Emerald_MRVR/Interfaces/BuildingsModuleInterface.h"
#include "BuildingsModuleComponent.generated.h"


class AMilitaryBase;
class UBuildingDataAsset;
class UStaticMeshComponent;
class UBoxComponent; // ?? Uvidime

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UBuildingsModuleComponent : public USceneComponent, public IBuildingsModuleInterface
{
	GENERATED_BODY()

public:	
	UBuildingsModuleComponent();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Building")
	UStaticMeshComponent* ModuleMesh;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Building")
	UBuildingDataAsset* BuildingDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Base")
	TObjectPtr<AMilitaryBase> MyBaseInstance;

	UFUNCTION()
	virtual void HighlightModule(bool bIsHighlighted) override;

	UFUNCTION(Server, Unreliable)
	void Server_HighlightModule(UMaterialInterface* Material);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_HighlightModule(UMaterialInterface* Material);

	bool CurrentlyHighlightedState = false;
	


	
};
