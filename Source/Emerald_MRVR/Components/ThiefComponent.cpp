#include "ThiefComponent.h"

UThiefComponent::UThiefComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UThiefComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UThiefComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

