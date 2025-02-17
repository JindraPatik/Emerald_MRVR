#include "CombatComponent.h"

#include "CollaborantComponent.h"
#include "HarvestComponent.h"
#include "HealthComponent.h"
#include "ThiefComponent.h"
#include "UnitMovementComponent.h"
#include "Emerald_MRVR/Actors/MilitaryBase.h"
#include "Emerald_MRVR/Actors/Unit.h"
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
		Unit->Body->OnComponentBeginOverlap.AddDynamic(this, &UCombatComponent::OnBoxOverlapped);
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
		UHarvestComponent* MyHarvestComponent = GetOwner()->FindComponentByClass<UHarvestComponent>();
		UThiefComponent* MyThiefComponent = GetOwner()->FindComponentByClass<UThiefComponent>();
		UHarvestComponent* OtherHarvestComponent = InActor->FindComponentByClass<UHarvestComponent>();
		UThiefComponent* OtherThiefComponent = InActor->FindComponentByClass<UThiefComponent>();
		UCollaborantComponent* CollaborantComponent = InActor->FindComponentByClass<UCollaborantComponent>();
		
		if (HittedUnit && HittedUnit->GetOwner() == GetOwner()->GetOwner()) 
		{
			return;
		}

		if (MyThiefComponent && OtherHarvestComponent && OtherHarvestComponent->bIsLoaded)
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
		                {
		                	UUnitMovementComponent* MyUnitMovementComponent = Unit->FindComponentByClass<UUnitMovementComponent>();
		                	if (MyUnitMovementComponent)
		                	{
								if (!HittedUnit->bIsFlyingUnit)
								{
		                			MyUnitMovementComponent->StopUnit();
									GetWorld()->GetTimerManager().SetTimer(FightSequenceHandle, MyUnitMovementComponent, &UUnitMovementComponent::RestartMovement, Unit->FightDelay, false);
								}
		                		HittedUnit->KillMe();
				                
		                	}
		                }
                		return;
                	}

                	if (Unit->Strenght == HittedUnit->Strenght) // Tie condition
                	{
                		CurrentScenario = ECombatScenarios::E_Tie;
                		HittedUnit->KillMe();
                		Unit->KillMe();
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












