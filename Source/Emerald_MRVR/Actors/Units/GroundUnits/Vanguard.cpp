#include "Vanguard.h"


AVanguard::AVanguard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
