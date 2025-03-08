#include "Jet.h"

AJet::AJet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AJet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

