#include "CommonPawn.h"

#include "Emerald_MRVR/Actors/Resources/PowerUp/PowerUp.h"

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
	if (!InPowerUp)
	{
		return;
	}
	AvailablePowerUps.Add(InPowerUp);
	UE_LOG(LogTemp, Warning, TEXT("AVRPawn::AddPowerUp Added"));
}

void ACommonPawn::ActivatePowerUp()
{
	if (!SelectedPowerUp)
	{
		return;
	}
	AvailablePowerUps.Remove(SelectedPowerUp);
	SelectedPowerUp->Activate();
	SelectedPowerUp = nullptr;
}

void ACommonPawn::CyclePowerUps()
{
	if (AvailablePowerUps.Num() > 0)
	{
		if (PowerUpIndex <= AvailablePowerUps.Num() - 1)
		{
			SelectedPowerUp = AvailablePowerUps[PowerUpIndex];
			PowerUpIndex++;
			UE_LOG(LogTemp, Warning, TEXT("AVRPawn::CyclePowerUps %s"), *SelectedPowerUp.GetName());
		}
		else
		{
			PowerUpIndex = 0;
			UE_LOG(LogTemp, Warning, TEXT("AVRPawn::CyclePowerUps %s"), *SelectedPowerUp.GetName());
		}
	}
}

