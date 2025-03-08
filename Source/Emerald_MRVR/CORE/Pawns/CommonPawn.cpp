#include "CommonPawn.h"

#include "Emerald_MRVR/Actors/Resources/PowerUp/PowerUp.h"

ACommonPawn::ACommonPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

void ACommonPawn::TogglePlayerInputEnabled()
{
	bInputIsEnabled = !bInputIsEnabled;
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
		// Přidáme modulo (%) pro plynulý cyklus
		PowerUpIndex = (PowerUpIndex + 1) % AvailablePowerUps.Num();
		SelectedPowerUp = AvailablePowerUps[PowerUpIndex];

		UE_LOG(LogTemp, Warning, TEXT("AVRPawn::CyclePowerUps %s"), *SelectedPowerUp.GetName());
	}
}

