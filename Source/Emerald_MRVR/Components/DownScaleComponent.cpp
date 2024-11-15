#include "DownScaleComponent.h"


UDownScaleComponent::UDownScaleComponent()
{
	DownscaleFactor = 1.f;
}


void UDownScaleComponent::BeginPlay()
{
	Super::BeginPlay();
	DownscaleOwner();
}


void UDownScaleComponent::DownscaleOwner()
{
	AActor* OwningActor = GetOwner();
	if (OwningActor)
	{
		USceneComponent* RootComponent = OwningActor->GetRootComponent();
		FVector DownscaleVector = DownscaleFactor * FVector(1.f, 1.f, 1.f);
		RootComponent->SetWorldScale3D(DownscaleVector);
	}
}



