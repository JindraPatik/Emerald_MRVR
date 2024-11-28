#include "ResourcesComponent.h"
#include "Components/TextBlock.h"
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
		UE_LOG(LogTemp, Warning, TEXT("ResourcesChanged"));
	}
}

void UResourcesComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UResourcesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UResourcesComponent::UpdateResources(float ResourcesDelta)
{
	AvailableResources -= ResourcesDelta;
}
