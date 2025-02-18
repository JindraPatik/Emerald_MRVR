#include "Brigade.h"


ABrigade::ABrigade()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABrigade::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABrigade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
