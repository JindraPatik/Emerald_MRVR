#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingsModuleComponent.generated.h"


class UBuildingDataAsset;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UBuildingsModuleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuildingsModuleComponent();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
