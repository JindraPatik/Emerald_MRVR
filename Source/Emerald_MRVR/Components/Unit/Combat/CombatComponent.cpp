#include "CombatComponent.h"

#include "Emerald_MRVR/Components/Health/HealthComponent.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"
#include "Emerald_MRVR/Actors/MilitaryBase.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Emerald_MRVR/Actors/Units/SpecialUnits/Collaborator.h"
#include "Emerald_MRVR/Actors/Units/SpecialUnits/Harvester.h"
#include "Emerald_MRVR/Actors/Units/SpecialUnits/Thief.h"
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
	
	if (InActor)
	{
		APawn* VR_Pawn = Cast<APawn>(GetOwner()->GetOwner());
		AUnit* HittedUnit = Cast<AUnit>(InActor);
		
		AHarvester* Harvester = Cast<AHarvester>(GetOwner());
		AHarvester* OtherHarvester = Cast<AHarvester>(InActor);
		
		AThief* Thief = Cast<AThief>(GetOuter());
		AThief* OtherThief = Cast<AThief>(InActor);
		
		ACollaborator* Collaborator = Cast<ACollaborator>(InActor);

		if (Collaborator)
		{
			return;
		}
		
		if (HittedUnit && HittedUnit->GetOwner() == GetOwner()->GetOwner()) 
		{
			return;
		}

		if (Thief && OtherHarvester && OtherHarvester->bIsLoaded)
		{
			return;
		}
		
		if (HittedUnit)
		{
			APawn* OtherOwner = Cast<APawn>(HittedUnit->GetOwner());
			if (OtherOwner)
			{
				if (VR_Pawn && HittedUnit && OtherOwner && VR_Pawn != OtherOwner) 	// Is that Other players Unit? 
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

void UCombatComponent::BaseFight(AActor* InActor)
{
	APawn* VR_Pawn = Cast<APawn>(GetOwner()->GetOwner());
	AMilitaryBase* HittedBase = Cast<AMilitaryBase>(InActor);
	AThief* Thief = Cast<AThief>(GetOwner());

	if (Thief && HittedBase && HittedBase->GetOwner() != GetOwner()->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Thief && HittedBase && HittedBase->GetOwner() != GetOwner()->GetOwner()"));
		return; // If Im a thief dont kill me
	}
	
	if (!HittedBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("HittedBase"));
		return;
	}
	
	APawn* OtherBaseOwner = Cast<APawn>(HittedBase->GetOwner());
	if (!OtherBaseOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("!OtherBaseOwner"));
		return;
	}
	if (VR_Pawn && HittedBase && OtherBaseOwner && OtherBaseOwner != VR_Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("VR_Pawn && HittedBase && OtherBaseOwner && OtherBaseOwner != VR_Pawn"));
		UHealthComponent* OtherBaseHealthComp = OtherBaseOwner->FindComponentByClass<UHealthComponent>();

		if (!OtherBaseHealthComp)
		{
			return;
		}
		
		OtherBaseHealthComp->Health -= Unit->Damage;
		OtherBaseHealthComp->Health = FMath::Clamp(OtherBaseHealthComp->Health, 0, OtherBaseHealthComp->MaxHealth);
		OtherBaseHealthComp->OnRep_OnHealthChanged();
		GetOwner()->Destroy();
	}
}












