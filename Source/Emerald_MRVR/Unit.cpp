#include "Unit.h"

#include "MilitaryBase.h"
#include "Components/UnitMovementComponent.h"
#include "CORE/MR_General.h"
#include "CORE/PC_MR_General.h"
#include "CORE/MR_General.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = Body;
	bReplicates = true;
	UnitMovementComponent = CreateDefaultSubobject<UUnitMovementComponent>("UnitMovementComponent");
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();
	TargetLoc = FVector(0.f, 0.f, 0.f);

}

void AUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUnit, Speed)
	DOREPLIFETIME(AUnit, Body)
	DOREPLIFETIME(AUnit, TargetLoc)
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UnitMovementComponent->bMovementEnabled)
	{
		UnitMovementComponent->MoveTo(DeltaTime);	
	}

}

void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

