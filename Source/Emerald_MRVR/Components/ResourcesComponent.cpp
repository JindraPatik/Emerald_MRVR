#include "ResourcesComponent.h"

#include "MilitaryBaseComp.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/CORE/MR_General.h"
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



void UResourcesComponent::OnRep_ResourcesChanged() const
{
	AMR_General* General = Cast<AMR_General>(GetOwner());
	if (General)
	{
		if (General->MilitaryBaseComp)
		{
			AMilitaryBase* BaseInstance = General->MilitaryBaseComp->GetBaseInstance();
			if (BaseInstance && BaseInstance->ResourcesWidgetInstance)
			{
								
				UResourcesWidget* ResourcesWidget = Cast<UResourcesWidget>(BaseInstance->ResourcesWidgetInstance->FindComponentByClass<UWidgetComponent>()->GetWidget());
				if (ResourcesWidget)
				{
					ResourcesWidget->UpdateResourcesWidget(FMath::Clamp(AvailableResources, 0.f, MaxResources));
				}
			}
		}
	}
	
}

void UResourcesComponent::UpdateResources(float ResourcesDelta)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		AvailableResources = FMath::Max(0, AvailableResources - ResourcesDelta);
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
