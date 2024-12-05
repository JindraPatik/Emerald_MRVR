#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Emerald_MRVR/Interfaces/BuildingsModuleInterface.h"
#include "BuildingsModuleComponent.generated.h"


class AMilitaryBase;
class UBuildingDataAsset;
class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UBuildingsModuleComponent : public UPrimitiveComponent, public IBuildingsModuleInterface
{
	GENERATED_BODY()

public:	
	UBuildingsModuleComponent();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitializeComponent() override;
public:	

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Building")
	UBuildingDataAsset* BuildingDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Base")
	TObjectPtr<AMilitaryBase> MyBaseInstance;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Building")
	UStaticMeshComponent* ModuleMeshInstance;

	UFUNCTION()
	virtual void HighlightModule(bool bIsHighlighted) override;

	bool CurrentlyHighlightedState = false;
	


	
};
