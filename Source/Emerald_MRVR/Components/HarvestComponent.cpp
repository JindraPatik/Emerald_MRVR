#include "HarvestComponent.h"
#include "ResourcesComponent.h"
#include "UnitMovementComponent.h"
#include "Emerald_MRVR/Crystal.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/Unit.h"


UHarvestComponent::UHarvestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UHarvestComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Unit = Cast<AUnit>(GetOwner());
	if (Unit)
	{
		
		Unit->Body->OnComponentBeginOverlap.AddDynamic(this, &UHarvestComponent::OnBoxOverlap);
	}
}


void UHarvestComponent::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CollectCrystal(OtherActor);
	DeliverCrystal(OtherActor);
}

void UHarvestComponent::CollectCrystal(AActor* HittedActor)
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

void UHarvestComponent::Server_CollectCrystal_Implementation(AActor* HitActor)
{
	CollectCrystal(HitActor);
}

void UHarvestComponent::DeliverCrystal(AActor* HitActor)
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

void UHarvestComponent::Server_DeliverCrystal_Implementation(AActor* HitActor)
{
	DeliverCrystal(HitActor);
}












