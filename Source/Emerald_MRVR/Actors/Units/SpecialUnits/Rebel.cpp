#include "Rebel.h"

#include "Emerald_MRVR/Actors/Units/GroundUnits/Blaster.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"


ARebel::ARebel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARebel::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ARebel::OnOverlapped);
	
}

void ARebel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARebel::OnOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	AUnit* OtherUnit = Cast<AUnit>(OtherActor);
	if (OtherUnit && OtherUnit->GetOwner() != GetOwner())
	{
		if (ABlaster* Blaster = Cast<ABlaster>(OtherUnit))
		/* If contacts with transformed mine let the mine explode */
		{
			if (Blaster->bBlasterIsTransformed)
			{
				Blaster->Explode(GetOwner());
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
