
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MilitaryBase.generated.h"

class UHealthComponent;
class UBuildingsModuleComponent;
class UDownScaleComponent;
class UBoxComponent;

UCLASS()
class EMERALD_MRVR_API AMilitaryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMilitaryBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "BaseBody")
	TObjectPtr<UStaticMeshComponent> BaseBody;

	UPROPERTY(EditAnywhere, Category = "BaseBody")
	TObjectPtr<UBoxComponent> BaseBox;

	UPROPERTY(VisibleDefaultsOnly, Category="BaseBody")
	UDownScaleComponent* DownScaleComponent;

	TObjectPtr<UBuildingsModuleComponent> BuildingsModuleComponent;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
