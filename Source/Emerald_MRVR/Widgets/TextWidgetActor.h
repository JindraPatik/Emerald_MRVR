#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TextWidgetActor.generated.h"

class UWidgetComponent;

UCLASS()
class EMERALD_MRVR_API ATextWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATextWidgetActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Widget")
	UWidgetComponent* Widget;

public:	
	virtual void Tick(float DeltaTime) override;

	
	
};
