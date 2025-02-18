#include "Collaborator.h"

#include "Emerald_MRVR/Actors/Units/GroundUnits/Mortar.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"


ACollaborator::ACollaborator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACollaborator::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACollaborator::OnOverlapped);
	
}

void ACollaborator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollaborator::OnOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	AUnit* OtherUnit = Cast<AUnit>(OtherActor);
	if (OtherUnit && OtherUnit->GetOwner() != GetOwner())
	{
		if (AMortar* MortarUnit = Cast<AMortar>(OtherUnit))
		/* If contacts with transformed mine let the mine explode */
		{
			if (MortarUnit->bMortarIsTransformed)
			{
				MortarUnit->Explode(GetOwner());
				return;
			}
		}
		
		OtherActor->Owner = GetOwner(); // Converts Unit ownership from Enemy to Player

		/* Turns direction of converted Unit */
		UUnitMovementComponent* OtherUnitMovementComp = OtherActor->FindComponentByClass<UUnitMovementComponent>();
		if (!OtherUnitMovementComp)
		{
			return;
		}
		
		OtherUnitMovementComp->Turn180();

		if (!UnitMovementComponent)
		{
			return;
		}

		// TODO : Change to return sequence
		KillMe();
	}
}
