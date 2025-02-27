#include "Digger.h"

#include "Emerald_MRVR/Actors/Resources/Crystal.h"
#include "Emerald_MRVR/Actors/MilitaryStation/MilitaryStation.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"


ADigger::ADigger()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADigger::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ADigger::OnOverlapped);
}

void ADigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADigger::OnOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	CollectCrystal(OtherActor);
	DeliverCrystal(OtherActor);
}

void ADigger::CollectCrystal(AActor* HittedActor)
{
	ACrystal* Crystal = Cast<ACrystal>(HittedActor);
	if (!Crystal)
	{
		return;
	}
	
	if (bIsLoaded)
	{
		return; // If already Loaded with resources, ignore crystal
	}

	DiggedValue = Crystal->CrystalValue;
	Crystal->Destroy(); // TODO : Substitue with some KillMe() Crystal function
	
	bIsLoaded = true;
	UnitMovementComponent->Turn180();
}

void ADigger::DeliverCrystal(AActor* HitActor)
{
	AMilitaryStation* MilitaryStation = Cast<AMilitaryStation>(HitActor);
	if (!MilitaryStation)
	{
		return;
	}
	
	APawn* VR_Pawn = Cast<APawn>(GetOwner());
	APawn* BaseOwner = Cast<APawn>(MilitaryStation->GetOwner());

	if (VR_Pawn && BaseOwner && VR_Pawn == BaseOwner && bIsLoaded)
	{
		UResourcesComponent* ResourcesComponent = VR_Pawn->FindComponentByClass<UResourcesComponent>();
		if (!ResourcesComponent)
		{
			return;
		}
		
		ResourcesComponent->AvailableResources += DiggedValue;
		bIsLoaded = false;
		KillMe();
	}
}


