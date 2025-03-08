#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CooldownActor.generated.h"

class UWidgetComponent;

UCLASS()
class EMERALD_MRVR_API ACooldownActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACooldownActor(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UWidgetComponent> Widget; 

public:	
	virtual void Tick(float DeltaTime) override;

	
	
};
