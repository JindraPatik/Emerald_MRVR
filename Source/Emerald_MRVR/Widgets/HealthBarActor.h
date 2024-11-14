#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthBarActor.generated.h"

class UWidgetComponent;

UCLASS()
class EMERALD_MRVR_API AHealthBarActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealthBarActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Widget")
	UWidgetComponent* Widget;
	

public:	
	virtual void Tick(float DeltaTime) override;

	
	
};
