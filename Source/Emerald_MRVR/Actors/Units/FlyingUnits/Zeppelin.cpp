#include "Zeppelin.h"

AZeppelin::AZeppelin(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZeppelin::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZeppelin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZeppelin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

