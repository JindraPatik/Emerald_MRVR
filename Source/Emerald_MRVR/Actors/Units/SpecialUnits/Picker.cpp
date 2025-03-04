#include "Picker.h"

#include "Emerald_MRVR/Actors/MilitaryStation/MilitaryStation.h"
#include "Emerald_MRVR/Actors/Resources/PowerUp.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"
#include "Emerald_MRVR/CORE/Pawns/AIPawn.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"


APicker::APicker()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APicker::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APicker::OnOverlapped);
	
}


void APicker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APicker::OnOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	APowerUp* PowerUp = Cast<APowerUp>(OtherActor);
	AVRPawn* VRPawn = Cast<AVRPawn>(GetOwner());
	AAIPawn* AIPawn = Cast<AAIPawn>(GetOwner());
	

	if (!VRPawn && !AIPawn)
	{
		return;
	}
	
	if (!PowerUp)
	{
		return;
	}

	/* Pick PowerUp */
	if (!bIsLoaded)
	{
		PickPowerUp(PowerUp);
		return;
	}
	
	/* Deliver PowerUp */
	if (bIsLoaded && Cast<AMilitaryStation>(OtherActor) == VRPawn->MilitaryStationInstance || Cast<AMilitaryStation>(OtherActor) == AIPawn->MilitaryStationInstance)
	{
		if (VRPawn)
		{
			VRPawn->AddPowerUp(PowerUp);
			UE_LOG(LogTemp, Warning, TEXT("Pickup Delivered"));
		}
		if (AIPawn)
		{
			AIPawn->AddPowerUp(PowerUp);
			UE_LOG(LogTemp, Warning, TEXT("Pickup Delivered"));
		}
		
		Destroy();
	}

}

void APicker::PickPowerUp(APowerUp* InPowerUp)
{
	bIsLoaded = true;
	UnitMovementComponent->Turn180();
	InPowerUp->Destroy();
}



