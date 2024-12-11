#include "HealthComponent.h"

#include "MilitaryBaseComp.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/Widgets/HealthBarWidget.h"
#include "Net/UnrealNetwork.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	Health = MaxHealth;
}



void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::OnRep_OnHealthChanged()
{
	AMR_General* General = Cast<AMR_General>(GetOwner());
	if (General)
	{
		if (General->MilitaryBaseComp)
		{
			AMilitaryBase* BaseInstance = General->MilitaryBaseComp->GatBaseInstance();
			if (BaseInstance && BaseInstance->HealthWidgetInstance)
			{
								
				UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(BaseInstance->HealthWidgetInstance->FindComponentByClass<UWidgetComponent>()->GetWidget());
				if (HealthBarWidget)
				{
					HealthBarWidget->UpdateHealthWidget(Health);
				}
			}
		}
	}
}
