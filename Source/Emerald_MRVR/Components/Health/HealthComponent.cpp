#include "HealthComponent.h"

#include "Emerald_MRVR/Components/MilitaryStation/MilitaryStationComp.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/Actors/MilitaryStation/MilitaryStation.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"
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
	MilitaryBaseCompInstance = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
}

void UHealthComponent::OnRep_OnHealthChanged()
{
	if (!MilitaryBaseCompInstance)
	{
		return;
	}
	
	AMilitaryStation* MilitaryBaseInstance = MilitaryBaseCompInstance->GetMilitaryStationInstance();
	if (MilitaryBaseInstance && MilitaryBaseInstance->HealthWidgetInstance)
	{
		UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(MilitaryBaseInstance->HealthWidgetInstance->FindComponentByClass<UWidgetComponent>()->GetWidget());
		if (HealthBarWidget)
		{
			HealthBarWidget->UpdateHealthWidget(Health);
		}
		if (Health <= 0)
		{
			AGameModeCommon* GameModeCommon = Cast<AGameModeCommon>(GetWorld()->GetAuthGameMode());
			if (GameModeCommon)
			{
				GameModeCommon->EndGame(Cast<APawn>(GetOwner()));
			}
		}
	}
}
