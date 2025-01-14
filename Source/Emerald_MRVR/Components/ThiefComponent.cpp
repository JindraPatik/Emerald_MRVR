#include "ThiefComponent.h"

#include "BoxComponent.h"
#include "CombatComponent.h"
#include "HarvestComponent.h"
#include "ResourcesComponent.h"
#include "UnitMovementComponent.h"
#include "Emerald_MRVR/Unit.h"

UThiefComponent::UThiefComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UThiefComponent::BeginPlay()
{
	Super::BeginPlay();

	Unit = Cast<AUnit>(GetOwner());
	if (Unit)
	{
		Unit->BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UThiefComponent::OnBoxOverlapped);
	}
}

void UThiefComponent::OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		HarvesterRobbery(OtherActor);
		BaseRobbery(OtherActor);
	}
}

void UThiefComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UThiefComponent::HarvesterRobbery(AActor* OtherActor)
{
	if (OtherActor && (OtherActor->GetOwner() != GetOwner()->GetOwner())) // Not my Unit
	{
		UHarvestComponent* HarvestComponent = OtherActor->FindComponentByClass<UHarvestComponent>();
		UCombatComponent* CombatComponent = OtherActor->FindComponentByClass<UCombatComponent>();
		UUnitMovementComponent* UnitMovementComponent = GetOwner()->FindComponentByClass<UUnitMovementComponent>();
		
		if (HarvestComponent) // Is harvester
		{
			if (CombatComponent && HarvestComponent->bIsLoaded) // Loaded harvester
			{
				CombatComponent->bWillFight = false; // Disable Combat
				StealedValue = HarvestComponent->HarvestedValue; // Get stealed amount
				bIsloaded = true;
				OtherActor->Destroy(); // Destroy enemy loaded harvester
				if (UnitMovementComponent)
				{
					UnitMovementComponent->UnitSpeed = -UnitMovementComponent->UnitSpeed; // Change Unit direction. LATER SUBSTITUTE WITH TURN180 FUNCTION!!
				}
			}
		}
	}
}

void UThiefComponent::BaseRobbery(AActor* OtherActor)
{
	if (OtherActor && OtherActor->GetOwner() && OtherActor->GetOwner() != GetOwner()->GetOwner()) // Not me
	{
		/*UHarvestComponent* HarvestComponent = OtherActor->FindComponentByClass<UHarvestComponent>();
		if (HarvestComponent)
		{
			return;
		}*/

		UResourcesComponent* EnemyResourcesComponent = OtherActor->GetOwner()->FindComponentByClass<UResourcesComponent>();
		
		if (EnemyResourcesComponent)
		{
			if (EnemyResourcesComponent->AvailableResources >= AmountToStealFromBase) // Enemy has enough to steal
			{
				StealedValue = AmountToStealFromBase;
				bIsloaded = true;
				EnemyResourcesComponent->AvailableResources -= AmountToStealFromBase;
				
				UUnitMovementComponent* UnitMovementComponent = GetOwner()->FindComponentByClass<UUnitMovementComponent>();
				if (UnitMovementComponent)
				{
					UnitMovementComponent->UnitSpeed = -UnitMovementComponent->UnitSpeed; // Change Unit direction. LATER SUBSTITUTE WITH TURN180 FUNCTION!!
				}
			}
			else
			{
				StealedValue = EnemyResourcesComponent->AvailableResources;
				EnemyResourcesComponent->AvailableResources -= EnemyResourcesComponent->AvailableResources;
				bIsloaded = true;
				
				UUnitMovementComponent* UnitMovementComponent = GetOwner()->FindComponentByClass<UUnitMovementComponent>();
				if (UnitMovementComponent)
				{
					UnitMovementComponent->UnitSpeed = -UnitMovementComponent->UnitSpeed; // Change Unit direction. LATER SUBSTITUTE WITH TURN180 FUNCTION!!
				}
			}
		}
	}
	else if (OtherActor && OtherActor->GetOwner() == GetOwner()->GetOwner() && bIsloaded)
	{
		UResourcesComponent* MyResourcesComponent = GetOwner()->GetOwner()->FindComponentByClass<UResourcesComponent>();
		if (MyResourcesComponent)
		{
			MyResourcesComponent->AvailableResources += StealedValue;
			GetOwner()->Destroy();
		}
		
	}
}


