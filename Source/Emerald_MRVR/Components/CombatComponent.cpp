#include "CombatComponent.h"

#include "BoxComponent.h"
#include "CollaborantComponent.h"
#include "HarvestComponent.h"
#include "HealthComponent.h"
#include "ThiefComponent.h"
#include "UnitMovementComponent.h"
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
		APawn* MyGeneral = Cast<APawn>(GetOwner()->GetOwner());
		AUnit* HittedUnit = Cast<AUnit>(InActor);
		UHarvestComponent* HarvestComponent = InActor->FindComponentByClass<UHarvestComponent>();
		UThiefComponent* ThiefComponent = GetOwner()->FindComponentByClass<UThiefComponent>();
		UCollaborantComponent* CollaborantComponent = InActor->FindComponentByClass<UCollaborantComponent>();
		
		if (HarvestComponent && ThiefComponent && InActor->GetOwner() != GetOwner()->GetOwner() && HarvestComponent->bIsLoaded) return; 
		if (CollaborantComponent) return; // Ignore fight for Collaborant

		AUnit* InUnit = Cast<AUnit>(InActor);
		
		if (InUnit && InUnit->GetOwner() == GetOwner()->GetOwner()) // If it is my loaded thief
		{
			return;
		}
		
		if (HittedUnit)
		{
			APawn* OtherOwner = Cast<APawn>(HittedUnit->GetOwner());
			if (OtherOwner)
			{
				if (MyGeneral && HittedUnit && OtherOwner && MyGeneral != OtherOwner) 	// Is that Other players Unit? 
                {
                	if (Unit->Strenght > HittedUnit->Strenght) // Win condition
                	{
                		CurrentScenario = ECombatScenarios::E_Win;
		                if (!InUnit->bIsFlyingUnit)
		                {
		                	UUnitMovementComponent* MyUnitMovementComponent = Unit->FindComponentByClass<UUnitMovementComponent>();
		                	if (MyUnitMovementComponent)
		                	{
								MyUnitMovementComponent->bMovementEnabled = false;
		                		MyUnitMovementComponent->StopUnit();
                				InActor->Destroy();
		                		GetWorld()->GetTimerManager().SetTimer(FightSequenceHandle, MyUnitMovementComponent, &UUnitMovementComponent::RestartMovement, Unit->FightDelay, false);
		                	}
		                }
                		return;
                	}

                	if (Unit->Strenght == HittedUnit->Strenght) // Tie condition
                	{
                		CurrentScenario = ECombatScenarios::E_Tie;
                		InActor->Destroy();
                		GetOwner()->Destroy();
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

	APawn* MyGeneral = Cast<APawn>(GetOwner()->GetOwner());
	AMilitaryBase* HittedBase = Cast<AMilitaryBase>(InActor);
	
	UThiefComponent* ThiefComponent = GetOwner()->FindComponentByClass<UThiefComponent>();
	
	if (ThiefComponent && HittedBase && HittedBase->GetOwner() != GetOwner()->GetOwner()) return; // If Im a thief dont kill me
	
	if (HittedBase)
	{
		APawn* OtherBaseOwner = Cast<APawn>(HittedBase->GetOwner());
		if (OtherBaseOwner)
		{
			if (MyGeneral && HittedBase && OtherBaseOwner && OtherBaseOwner != MyGeneral)
			{
				UHealthComponent* OtherBaseHealthComp = OtherBaseOwner->FindComponentByClass<UHealthComponent>();
				if (OtherBaseHealthComp)
				{
					OtherBaseHealthComp->Health -= Unit->Damage;
					OtherBaseHealthComp->Health = FMath::Clamp(OtherBaseHealthComp->Health, 0, OtherBaseHealthComp->MaxHealth);
					OtherBaseHealthComp->OnRep_OnHealthChanged();
					GetOwner()->Destroy();
				}
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












