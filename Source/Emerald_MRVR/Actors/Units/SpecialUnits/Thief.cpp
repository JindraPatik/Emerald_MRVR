#include "Thief.h"

#include "Digger.h"
#include "Emerald_MRVR/Actors/MilitaryStation/MilitaryStation.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"
#include "Emerald_MRVR/Components/Unit/Combat/CombatComponent.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"

class UResourcesComponent;

AThief::AThief(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	DiggerRobbery(OtherActor);
	BaseRobbery(OtherActor);
}

/* Steals resources from loaded Digger and return to base */
void AThief::DiggerRobbery(AActor* OtherActor)
{
	if (OtherActor && OtherActor->GetOwner() != GetOwner()) // Not my Unit
	{
		ADigger* Digger = Cast<ADigger>(OtherActor);
		UCombatComponent* CombatComponent = OtherActor->FindComponentByClass<UCombatComponent>();
		
		if (!Digger) // Is Digger
		{
			return;
		}
		
		if (CombatComponent && Digger->bIsLoaded) // Loaded Digger
		{
			StealedValue = Digger->DiggedValue; // Get stealed amount
			bIsloaded = true;
			OtherActor->Destroy(); // Destroy enemy loaded Digger
			
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

void AThief::BaseRobbery(AActor* OtherActor)
 {
	if (OtherActor && OtherActor->GetOwner() && OtherActor->GetOwner() != GetOwner()) // Not me
	{
		UResourcesComponent* EnemyResourcesComponent = OtherActor->GetOwner()->FindComponentByClass<UResourcesComponent>();
		if (!EnemyResourcesComponent)
		{
			return;
		}
		
		if (EnemyResourcesComponent->AvailableResources >= AmountToStealFromBase) // Enemy has enough to steal
		{
			StealedValue = AmountToStealFromBase;
			bIsloaded = true;
			EnemyResourcesComponent->AvailableResources -= AmountToStealFromBase;
			
			if (!UnitMovementComponent)
			{
				return;
			}
			
			ReturnSpeed = UnitMovementComponent->UnitSpeed - (UnitMovementComponent->UnitSpeed / 100) * ReturnSlowdownPercent;
			UnitMovementComponent->UnitSpeed = -ReturnSpeed; // Change Unit direction. LATER SUBSTITUTE WITH TURN180 FUNCTION!!
			return;
		}
		
		if (EnemyResourcesComponent->AvailableResources <= AmountToStealFromBase)
		{
			UE_LOG(LogTemp, Error, TEXT("EnemyResourcesComponent->AvailableResources <= AmountToStealFromBase"));
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
	
	else if (OtherActor && OtherActor->GetOwner() == GetOwner() && bIsloaded) // If is my base and Thief bIsLoaded -> Deliver
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

