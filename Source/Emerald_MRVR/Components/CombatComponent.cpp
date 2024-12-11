#include "CombatComponent.h"

#include "BoxComponent.h"
#include "HealthComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"

UENUM(BlueprintType)
enum class ECombatScenarios
{
	E_Win,
	E_Loose,
	E_Tie
};

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Unit = Cast<AUnit>(GetOwner());
	if (Unit)
	{
		Unit->BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UCombatComponent::OnBoxOverlapped);
	}
}

void UCombatComponent::OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UnitFight(OtherActor);
		BaseFight(OtherActor);
	}
}

void UCombatComponent::UnitFight(AActor* InActor)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_UnitFight(InActor);
		return;
	}
	
	if (InActor)
	{
		AMR_General* MyGeneral = Cast<AMR_General>(GetOwner()->GetOwner());
		AUnit* HittedUnit = Cast<AUnit>(InActor);
		if (HittedUnit)
		{
			AMR_General* OtherOwner = Cast<AMR_General>(HittedUnit->GetOwner());
			if (OtherOwner)
			{
				if (MyGeneral && HittedUnit && OtherOwner && MyGeneral != OtherOwner) 	// Is that Other players Unit? 
                {
                	if (Unit->Strenght > HittedUnit->Strenght) // Win condition
                	{
                		CurrentScenario = ECombatScenarios::E_Win;
                		InActor->Destroy();
                		
                		return;
                	}
                	if (Unit->Strenght < HittedUnit->Strenght) // Loose condition
                	{
                		CurrentScenario = ECombatScenarios::E_Loose;
                		GetOwner()->Destroy();
                		return;
                	}
                	if (Unit->Strenght == HittedUnit->Strenght) // Tie condition
                	{
                		CurrentScenario = ECombatScenarios::E_Tie;
                		return;
                	}
                }
				return;
			}
			return;
			
		}
		return;

	}
}

void UCombatComponent::Server_UnitFight_Implementation(AActor* InActor)
{
	UnitFight(InActor);
}

void UCombatComponent::BaseFight(AActor* InActor)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_BaseFight(InActor);
		return;
	}
	AMR_General* MyGeneral = Cast<AMR_General>(GetOwner()->GetOwner());
	AMilitaryBase* HittedBase = Cast<AMilitaryBase>(InActor);
	if (HittedBase)
	{
		AMR_General* OtherBaseOwner = Cast<AMR_General>(HittedBase->GetOwner());
		if (OtherBaseOwner)
		{
			if (MyGeneral && HittedBase && OtherBaseOwner && OtherBaseOwner != MyGeneral)
			{
				OtherBaseOwner->HealthComponent->Health -= Unit->Damage;
				GetOwner()->Destroy();
			}
			return;
		}
		return;
	}
	return;
}

void UCombatComponent::Server_BaseFight_Implementation(AActor* InActor)
{
	BaseFight(InActor);
}












