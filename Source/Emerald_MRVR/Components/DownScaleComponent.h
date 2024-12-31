
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DownScaleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UDownScaleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDownScaleComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Resize")
		float DownscaleFactor;
	
protected:
	virtual void BeginPlay() override;

	void DownscaleOwner();

public:	

		
	
};
