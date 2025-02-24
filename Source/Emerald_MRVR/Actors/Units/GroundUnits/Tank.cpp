#include "Tank.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

