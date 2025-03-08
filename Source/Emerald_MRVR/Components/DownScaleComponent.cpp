#include "DownScaleComponent.h"

#include "Emerald_MRVR/EmeraldKeeper.h"


UDownScaleComponent::UDownScaleComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DownscaleFactor = GLOBAL_DOWNSCALE_VALUE;
}


void UDownScaleComponent::BeginPlay()
{
	Super::BeginPlay();
	DownscaleOwner();
}


void UDownScaleComponent::DownscaleOwner()
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor)
	{
		return;
	}
	
	USceneComponent* RootComponent = OwningActor->GetRootComponent();
	FVector DownscaleVector = DownscaleFactor * FVector(1.f, 1.f, 1.f);
	RootComponent->SetWorldScale3D(DownscaleVector);
}



