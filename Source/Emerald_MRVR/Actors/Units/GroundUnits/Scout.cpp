#include "Scout.h"


AScout::AScout()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AScout::BeginPlay()
{
	Super::BeginPlay();
	
}

void AScout::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AScout::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

