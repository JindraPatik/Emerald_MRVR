#include "UnitMovementComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/CORE/PC_MR_General.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UUnitMovementComponent::UUnitMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bAutoActivate = true;
	SetIsReplicated(true);
}



void UUnitMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bMovementEnabled)
	{
		// MoveTo(DeltaTime);
	}
}


void UUnitMovementComponent::MoveTo(float DeltaTime) const
{
	AUnit* Unit = Cast<AUnit>(GetOwner());
	if (!Unit)
	{
		return;
	}
	
	FVector MovementDirection = FVector(1.f,0.f,0.f);
	FVector DeltaLocation = MovementDirection.GetSafeNormal() * (Unit->Speed * DeltaTime);
	Unit->GetRootComponent()->SetWorldLocation(DeltaLocation);

	// Až budu vědět jak získat lokaci druhé základny.
	/*FVector CurrentLoc =  Unit->GetActorLocation();
    	FVector Direction = (TargetLocation - CurrentLoc).GetSafeNormal();
    	float Speed = Unit->Speed;
    	FVector NewLocation = CurrentLoc + Direction * (Speed * DeltaTime);
    	Unit->SetActorLocation(NewLocation);*/
}

