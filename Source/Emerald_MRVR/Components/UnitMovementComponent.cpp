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
	DOREPLIFETIME(UUnitMovementComponent, UnitSpeed);
}

void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AUnit* Unit = Cast<AUnit>(GetOwner());
	UnitSpeed = Unit->Speed;
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UUnitMovementComponent::MoveTo(float DeltaTime) const
{
	AUnit* Unit = Cast<AUnit>(GetOwner());
	if (!Unit)
	{
		return;
	}

	if (bMovementEnabled)
	{
		FVector MovementDirection = Unit->GetActorForwardVector();
		FVector DeltaLocation = MovementDirection.GetSafeNormal() * (UnitSpeed * DeltaTime);
		Unit->AddActorWorldOffset(DeltaLocation);
	}
}

