#include "Unit.h"

#include "Components/UnitMovementComponent.h"


AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = Body;

	UnitMovementComponent = CreateDefaultSubobject<UUnitMovementComponent>("UnitMovementComponent");
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

