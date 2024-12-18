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
	MilitaryBaseCompInst = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
}

void UHealthComponent::OnRep_OnHealthChanged()
{
	if (GetOwner()->HasAuthority())
	{
		OnRep_OnHealthChanged();
	}
	
	if (MilitaryBaseCompInst)
	{
		AMilitaryBase* BaseInstance = MilitaryBaseCompInst->GetBaseInstance();
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
