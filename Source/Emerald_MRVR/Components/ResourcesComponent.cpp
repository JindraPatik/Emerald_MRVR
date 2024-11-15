#include "ResourcesComponent.h"
#include "Components/TextBlock.h"
#include "Emerald_MRVR/Widgets/ResourcesWidget.h"
#include "Net/UnrealNetwork.h"


void UResourcesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UResourcesComponent,AvailableResources);
}

UResourcesComponent::UResourcesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	AvailableResources = 100.f;
}


void UResourcesComponent::OnRep_ResourcesChanged() const
{
	ResourcesWidget->UpdateResourcesWidget(AvailableResources);
}

void UResourcesComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UResourcesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

