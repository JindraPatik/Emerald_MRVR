#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingsModuleComponent.generated.h"


class UBuildingDataAsset;
class UStaticMeshComponent;
class UBoxComponent; // ?? Uvidime

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UBuildingsModuleComponent : public USceneComponent
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

	// Komponenta pro statický mesh
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Building")
	UStaticMeshComponent* ModuleMesh;

	// Box komponenta pro kolize
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Building")
	UBoxComponent* BoxCollision;

	// Data Asset pro specifické informace o budově
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category="Building")
	UBuildingDataAsset* BuildingDataAsset;
	
};
