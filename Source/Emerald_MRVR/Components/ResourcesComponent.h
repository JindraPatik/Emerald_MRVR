#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourcesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UResourcesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UResourcesComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Resources")
	float AvailableResources;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
