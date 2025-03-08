#include "PathPoint.h"

APathPoint::APathPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APathPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

