#include "Helicopter.h"

AHelicopter::AHelicopter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHelicopter::BeginPlay()
{
	Super::BeginPlay();
}

void AHelicopter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

