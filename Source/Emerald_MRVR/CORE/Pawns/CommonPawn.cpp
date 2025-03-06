#include "CommonPawn.h"

#include "Emerald_MRVR/Actors/Resources/PowerUp.h"

ACommonPawn::ACommonPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACommonPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACommonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACommonPawn::AddPowerUp(APowerUp* InPowerUp)
{
	AvailablePowerUps.Add(InPowerUp);
	UE_LOG(LogTemp, Warning, TEXT("AVRPawn::AddPowerUp Added"));
}

void ACommonPawn::ActivatePowerUp()
{
	if (!SelectedPowerUp)
	{
		return;
	}
	SelectedPowerUp->Activate();
}

