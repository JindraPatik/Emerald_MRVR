#include "Vanguard.h"


AVanguard::AVanguard()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVanguard::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVanguard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
