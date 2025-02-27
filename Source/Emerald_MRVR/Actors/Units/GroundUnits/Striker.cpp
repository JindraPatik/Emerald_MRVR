#include "Striker.h"


AStriker::AStriker()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStriker::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStriker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


