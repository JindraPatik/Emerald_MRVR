#include "Helicopter.h"

AHelicopter::AHelicopter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

