#include "Voyager.h"


AVoyager::AVoyager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVoyager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVoyager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVoyager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

