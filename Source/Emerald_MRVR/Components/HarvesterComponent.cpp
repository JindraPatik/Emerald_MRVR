#include "HarvesterComponent.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"
#include "Emerald_MRVR/Actors/Crystal.h"
#include "Emerald_MRVR/Actors/MilitaryBase.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Unit/Movement/UnitMovementComponent.h"


UHarvesterComponent::UHarvesterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UHarvesterComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Unit = Cast<AUnit>(GetOwner());
	if (Unit)
	{
		
		Unit->Body->OnComponentBeginOverlap.AddDynamic(this, &UHarvesterComponent::OnBoxOverlap);
	}
}


void UHarvesterComponent::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CollectCrystal(OtherActor);
	DeliverCrystal(OtherActor);
}

void UHarvesterComponent::CollectCrystal(AActor* HittedActor)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_CollectCrystal(HittedActor);
		return;
	}
	
	ACrystal* Crystal = Cast<ACrystal>(HittedActor);
	if (Crystal)
	{
		if (bIsLoaded) return;

		HarvestedValue = Crystal->CrystalValue;
		Crystal->Destroy();
		
		bIsLoaded = true;
		Unit->UnitMovementComponent->Turn180();
	}
}

void UHarvesterComponent::Server_CollectCrystal_Implementation(AActor* HitActor)
{
	CollectCrystal(HitActor);
}

void UHarvesterComponent::DeliverCrystal(AActor* HitActor)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_CollectCrystal(HitActor);
		return;
	}
	
	AMilitaryBase* MilitaryBase = Cast<AMilitaryBase>(HitActor);
	if (MilitaryBase)
	{
		APawn* General = Cast<APawn>(GetOwner()->GetOwner());
		APawn* BaseOwner = Cast<APawn>(MilitaryBase->GetOwner());

		if (General && BaseOwner && General == BaseOwner && bIsLoaded)
		{
			UResourcesComponent* ResourcesComponent = General->FindComponentByClass<UResourcesComponent>();
			if (ResourcesComponent)
			{
				ResourcesComponent->AvailableResources += HarvestedValue;
				bIsLoaded = false;
				GetOwner()->Destroy();
			}
		}
	}
}

void UHarvesterComponent::Server_DeliverCrystal_Implementation(AActor* HitActor)
{
	DeliverCrystal(HitActor);
}












