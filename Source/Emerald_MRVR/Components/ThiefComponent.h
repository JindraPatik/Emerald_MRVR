#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThiefComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UThiefComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UThiefComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
