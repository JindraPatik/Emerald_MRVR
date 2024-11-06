
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MilitaryBase.generated.h"

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


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
