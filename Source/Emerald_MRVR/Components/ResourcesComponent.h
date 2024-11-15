#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourcesComponent.generated.h"

class UResourcesWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UResourcesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UResourcesComponent();

	UPROPERTY(ReplicatedUsing=OnRep_ResourcesChanged, EditAnywhere, BlueprintReadWrite, Category="Resources")
	float AvailableResources;

	UFUNCTION()
	void OnRep_ResourcesChanged() const;

	UPROPERTY(BlueprintReadWrite, Category="Widget")
	UResourcesWidget* ResourcesWidget;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
