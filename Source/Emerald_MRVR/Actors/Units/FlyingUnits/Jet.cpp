#include "Jet.h"

AJet::AJet()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AJet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJet::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

