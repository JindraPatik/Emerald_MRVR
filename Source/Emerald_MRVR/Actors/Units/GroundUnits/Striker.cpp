#include "Striker.h"


AStriker::AStriker(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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


