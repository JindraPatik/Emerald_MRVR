#include "ResourcesComponent.h"
#include "Emerald_MRVR/Widgets/ResourcesWidget.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarAddResources(TEXT("EKG.AddResources"), 5.f,TEXT("AddedResourcesToPlayer"), ECVF_Cheat);

void UResourcesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UResourcesComponent,AvailableResources);
}

UResourcesComponent::UResourcesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	AvailableResources = 100.f;
}

void UResourcesComponent::OnRep_ResourcesChanged() const
{
	if (ResourcesWidget)
	{
		ResourcesWidget->UpdateResourcesWidget(FMath::Clamp(AvailableResources, 0.f, MaxResources));
	}
}

void UResourcesComponent::UpdateResources(float ResourcesDelta)
{
	AvailableResources -= ResourcesDelta;
}
void UResourcesComponent::GrowResources()
{
	float ResourcesGrowAmount = 1.f;
	AvailableResources += ResourcesGrowAmount;
}

void UResourcesComponent::StartGrowResources()
{
	TObjectPtr<UWorld> World = GetWorld();
	FTimerHandle GrowResourcesTimerHandle;
	float GrowInterval = 1.f;
	if (World)
	{
		World->GetTimerManager().SetTimer(GrowResourcesTimerHandle, this, &UResourcesComponent::GrowResources, GrowInterval, true);
	}
}
