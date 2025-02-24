#include "CombatComponent.h"

#include "SplineComponent.h"
#include "Emerald_MRVR/Components/Health/HealthComponent.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"
#include "Emerald_MRVR/Actors/MilitaryBase/MilitaryStation.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Emerald_MRVR/Actors/Units/SpecialUnits/Collaborator.h"
#include "Emerald_MRVR/Actors/Units/SpecialUnits/Harvester.h"
#include "Emerald_MRVR/Actors/Units/SpecialUnits/Thief.h"
#include "Emerald_MRVR/Components/MilitaryBase/MilitaryStationComp.h"
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
	if (!Unit)
	{
		return;
	}
	Unit->OnActorBeginOverlap.AddDynamic(this, &UCombatComponent::OnActorOverlapped);
}

void UCombatComponent::OnActorOverlapped(AActor* OverlappedActor, AActor* OtherActor )
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

		/* if overlaps with Collaborator baypass fight */
		if (Collaborator)
		{
			return;
		}

		/* if overlaps with Player Unit baypass fight */
		if (HittedUnit && HittedUnit->GetOwner() == GetOwner()->GetOwner()) 
		{
			return;
		}

		/* if overlaps with Player loaded Harvester baypass fight */
		if (Thief && OtherHarvester && OtherHarvester->bIsLoaded || Harvester && Harvester->bIsLoaded && OtherThief)
		{
			return;
		}
		
		if (!HittedUnit)
		{
			return;
		}
		APawn* OtherOwner = Cast<APawn>(HittedUnit->GetOwner());
		if (!OtherOwner)
		{
			return;
		}
		if (VR_Pawn && HittedUnit && OtherOwner && VR_Pawn != OtherOwner) 	// Is that Other players Unit? 
        {
            if (Unit->Strenght > HittedUnit->Strenght) // Win condition
            {
                CurrentScenario = ECombatScenarios::E_Win;
                UUnitMovementComponent* UnitMovementComponent = Unit->FindComponentByClass<UUnitMovementComponent>();
                if (!UnitMovementComponent)
                {
	                return;
                }
                
				if (!HittedUnit->bIsFlyingUnit)
				{
	                UnitMovementComponent->StopUnit();
					GetWorld()->GetTimerManager().SetTimer(FightSequenceHandle, UnitMovementComponent, &UUnitMovementComponent::RestartMovement, Unit->FightDelay, false);
				}
                HittedUnit->KillMe();
                return;
            }

            if (Unit->Strenght == HittedUnit->Strenght) // Tie condition
            {
                CurrentScenario = ECombatScenarios::E_Tie;
                HittedUnit->KillMe();
                Unit->KillMe();
            }
        }
	}
}

void UCombatComponent::BaseFight(AActor* InActor)
{
	APawn* VR_Pawn = Cast<APawn>(GetOwner()->GetOwner());
	AMilitaryStation* HittedBase = Cast<AMilitaryStation>(InActor);
	AThief* Thief = Cast<AThief>(GetOwner());

	// If I'm a thief don't kill me
	if (Thief && HittedBase && HittedBase->GetOwner() != GetOwner()->GetOwner())
	{
		return; 
	}
	
	if (!HittedBase)
	{
		return;
	}
	
	APawn* OtherBaseOwner = Cast<APawn>(HittedBase->GetOwner());
	if (!OtherBaseOwner)
	{
		return;
	}

	/* If it is Enemy Base attack */
	if (VR_Pawn && HittedBase && OtherBaseOwner && OtherBaseOwner != VR_Pawn)
	{
		UHealthComponent* OtherBaseHealthComp = OtherBaseOwner->FindComponentByClass<UHealthComponent>();

		if (!OtherBaseHealthComp)
		{
			return;
		}
		
		OtherBaseHealthComp->Health -= Unit->Damage;
		OtherBaseHealthComp->Health = FMath::Clamp(OtherBaseHealthComp->Health, 0, OtherBaseHealthComp->MaxHealth);
		OtherBaseHealthComp->OnRep_OnHealthChanged();

		if (Unit->HasReturnPoint)
		{
			UUnitMovementComponent* UnitMovementComponent = Unit->FindComponentByClass<UUnitMovementComponent>();
			if (!UnitMovementComponent)
			{
				return;
			}

			int32 ConnectionPointIndex = UnitMovementComponent->MovementSpline->GetNumberOfSplinePoints() - 1;
			FTransform ConnectionPointTransform = FTransform(UnitMovementComponent->MovementSpline->GetRotationAtSplinePoint(ConnectionPointIndex, ESplineCoordinateSpace::World),
				UnitMovementComponent->MovementSpline->GetLocationAtSplinePoint(ConnectionPointIndex, ESplineCoordinateSpace::World));
			
			UnitMovementComponent->ExtendMovementPathToReturn(ConnectionPointTransform, Unit->OwningBuilding->UnitReturnPoint->GetComponentTransform());
			Unit->Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			return;
		}
		
		GetOwner()->Destroy();
	}
}












