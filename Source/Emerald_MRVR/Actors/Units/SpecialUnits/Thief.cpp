#include "Thief.h"

#include "Harvester.h"
#include "Emerald_MRVR/Actors/MilitaryBase/MilitaryStation.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"
#include "Emerald_MRVR/Components/Unit/Combat/CombatComponent.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"

class UResourcesComponent;

AThief::AThief()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AThief::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AThief::OnOverlapped);
}

void AThief::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AThief::OnOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor)
	{
		return;
	}
	HarvesterRobbery(OtherActor);
	StationRobbery(OtherActor);
}

/* Steals resources from loaded Harvester and return to station */
void AThief::HarvesterRobbery(AActor* OtherActor)
{
	if (OtherActor && OtherActor->GetOwner() != GetOwner()) // Not my Unit
	{
		AHarvester* Harvester = Cast<AHarvester>(OtherActor);
		UCombatComponent* CombatComponent = OtherActor->FindComponentByClass<UCombatComponent>();
		
		if (!Harvester) // Is harvester
		{
			return;
		}
		
		if (CombatComponent && Harvester->bIsLoaded) // Loaded harvester
		{
			StealedValue = Harvester->HarvestedValue; // Get stealed amount
			bIsloaded = true;
			OtherActor->Destroy(); // Destroy enemy loaded harvester
			
			if (!UnitMovementComponent)
			{
				return;
			}
			
			ReturnSpeed = UnitMovementComponent->UnitSpeed - (UnitMovementComponent->UnitSpeed / 100) * ReturnSlowdownPercent;
			UnitMovementComponent->UnitSpeed = -ReturnSpeed; // Change Unit direction. LATER SUBSTITUTE WITH TURN180 FUNCTION!!
		}
		else
		{
			KillMe();
		}
	}
}

void AThief::StationRobbery(AActor* OtherActor)
 {
	if (OtherActor && OtherActor->GetOwner() && OtherActor->GetOwner() != GetOwner()) // Not me
	{
		UResourcesComponent* EnemyResourcesComponent = OtherActor->GetOwner()->FindComponentByClass<UResourcesComponent>();
		if (!EnemyResourcesComponent)
		{
			return;
		}
		
		if (EnemyResourcesComponent->AvailableResources >= AmountToStealFromStation) // Enemy has enough to steal
		{
			StealedValue = AmountToStealFromStation;
			bIsloaded = true;
			EnemyResourcesComponent->AvailableResources -= AmountToStealFromStation;
			
			if (!UnitMovementComponent)
			{
				return;
			}
			
			ReturnSpeed = UnitMovementComponent->UnitSpeed - (UnitMovementComponent->UnitSpeed / 100) * ReturnSlowdownPercent;
			UnitMovementComponent->UnitSpeed = -ReturnSpeed; // Change Unit direction. LATER SUBSTITUTE WITH TURN180 FUNCTION!!
			return;
		}
		
		if (EnemyResourcesComponent->AvailableResources <= AmountToStealFromStation)
		{
			StealedValue = EnemyResourcesComponent->AvailableResources; // Steals rest of the resources Enemy has
			EnemyResourcesComponent->AvailableResources -= EnemyResourcesComponent->AvailableResources;
			bIsloaded = true;
			
			if (!UnitMovementComponent)
			{
				return;
			}
			
			ReturnSpeed = UnitMovementComponent->UnitSpeed - (UnitMovementComponent->UnitSpeed / 100) * ReturnSlowdownPercent;
			UnitMovementComponent->UnitSpeed = -ReturnSpeed; // Change Unit direction. LATER SUBSTITUTE WITH TURN180 FUNCTION!!
			
		}
	}
	
	else if (OtherActor && OtherActor->GetOwner() == GetOwner() && bIsloaded) // If is my Station and Thief bIsLoaded -> Deliver
	{
		AMilitaryStation* MilitaryStation = Cast<AMilitaryStation>(OtherActor);
		if (!MilitaryStation)
		{
			return;
		}
		
		UResourcesComponent* MyResourcesComponent = GetOwner()->FindComponentByClass<UResourcesComponent>();
		if (!MyResourcesComponent)
		{
			return;
		}
		
		MyResourcesComponent->AvailableResources += StealedValue;
		KillMe();
	}
}

