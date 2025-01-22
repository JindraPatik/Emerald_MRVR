#include "B25Component.h"


UB25Component::UB25Component()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UB25Component::BeginPlay()
{
	Super::BeginPlay();
	
}

void UB25Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

