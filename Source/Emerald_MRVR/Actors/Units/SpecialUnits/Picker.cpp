#include "Picker.h"

#include "Emerald_MRVR/Actors/MilitaryStation/MilitaryStation.h"
#include "Emerald_MRVR/Actors/Resources/PowerUp/PowerUp.h"
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
	APawn* Pawn = Cast<APawn>(GetOwner());

	if (!VRPawn && !AIPawn && !Pawn)
	{
		return;
	}
	
	/* Pick PowerUp */
	if (PowerUp && !bIsLoaded)
	{
		PickPowerUp(PowerUp, Pawn);
	}
	
	/* Deliver PowerUp */

	
	if (bIsLoaded && Cast<AMilitaryStation>(OtherActor) && OtherActor->GetOwner() == GetOwner() && PickedPowerUp)
	{
		if (VRPawn)
		{
			VRPawn->AddPowerUp(PickedPowerUp);
			UE_LOG(LogTemp, Warning, TEXT("Pickup Delivered"));
		}
		
		if (AIPawn)
		{
			AIPawn->AddPowerUp(PickedPowerUp);
			UE_LOG(LogTemp, Warning, TEXT("Pickup Delivered"));
		}
		
		Destroy();
	}

}

void APicker::PickPowerUp(APowerUp* InPowerUp, APawn* InOwner)
{
	bIsLoaded = true;
	UnitMovementComponent->Turn180();
	PickedPowerUp = InPowerUp;
	PickedPowerUp->PowerUpOwner = Cast<APawn>(GetOwner());
	InPowerUp->Destroy();
}



