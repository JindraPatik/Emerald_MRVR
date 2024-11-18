#include "Unit.h"

#include "Components/UnitMovementComponent.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = Body;
	// UnitMovementComponent = CreateDefaultSubobject<UUnitMovementComponent>("UnitMovementComponent");
	SetReplicates(true);
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
	SetOwner(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController());
}

void AUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME(AUnit, UnitMovementComponent)
	DOREPLIFETIME(AUnit, Speed)
	DOREPLIFETIME(AUnit, Body)
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

