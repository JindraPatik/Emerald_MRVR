#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingsModuleComponent.generated.h"


class UBuildingDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UBuildingsModuleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuildingsModuleComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Buildings")
	TArray<UBuildingDataAsset*> AvailableBuildings;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
