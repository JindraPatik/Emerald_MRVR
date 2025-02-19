#include "ResourcesComponent.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/Components/MilitaryBase/MilitaryStationComp.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"
#include "Emerald_MRVR/Widgets/HealthBarWidget.h"
#include "Emerald_MRVR/Widgets/ResourcesWidget.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarAddResources(TEXT("EKG.AddResources"), 5.f,TEXT("AddedResourcesToPlayer"), ECVF_Cheat);

UResourcesComponent::UResourcesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UResourcesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UResourcesComponent, AvailableResources);
}

void UResourcesComponent::BeginPlay()
{
	Super::BeginPlay();
	MilitaryBaseCompInst = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
	AGameModeCommon* GM = Cast<AGameModeCommon>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->OnGameEndedDelegate.AddDynamic(this, &UResourcesComponent::StopGrowResources);
	}
}

void UResourcesComponent::OnRep_ResourcesChanged() const
{
	if (MilitaryBaseCompInst)
	{
		AMilitaryStation* MilitaryBaseInstance = MilitaryBaseCompInst->GetMilitaryBaseInstance();
		if (MilitaryBaseInstance && MilitaryBaseInstance->ResourcesWidgetInstance)
		{
			UResourcesWidget* ResourcesWidget = Cast<UResourcesWidget>(MilitaryBaseInstance->ResourcesWidgetInstance->FindComponentByClass<UWidgetComponent>()->GetWidget());
			if (ResourcesWidget)
			{
				ResourcesWidget->UpdateResourcesWidget(FMath::Clamp(AvailableResources, 0.f, MaxResources));
			}
		}
	}
}

void UResourcesComponent::UpdateResources(float ResourcesDelta)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		AvailableResources = FMath::Max(0, AvailableResources - ResourcesDelta);
		OnRep_ResourcesChanged();
		return;
	}
	else
	{
		Server_UpdateResources(ResourcesDelta);
	}
}

void UResourcesComponent::Server_UpdateResources_Implementation(float ResourcesDelta)
{
	UpdateResources(ResourcesDelta);
}

void UResourcesComponent::GrowResources()
{
	float ResourcesGrowAmount = 1.f;
	AvailableResources += ResourcesGrowAmount;
	OnRep_ResourcesChanged();
}

void UResourcesComponent::StartGrowResources()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTimerHandle GrowResourcesTimerHandle;
		float GrowInterval = 2.f;
		World->GetTimerManager().SetTimer(GrowResourcesTimerHandle, this, &UResourcesComponent::GrowResources, GrowInterval, true);
	}
}

void UResourcesComponent::StopGrowResources(APawn* Looser)
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

