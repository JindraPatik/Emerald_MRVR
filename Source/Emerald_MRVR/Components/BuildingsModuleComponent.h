#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingsModuleComponent.generated.h"


class UBuildingDataAsset;
class UStaticMeshComponent;
class UBoxComponent; // ?? Uvidime

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UBuildingsModuleComponent : public UPrimitiveComponent
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


	
};
