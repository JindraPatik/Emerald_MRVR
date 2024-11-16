#include "UnitMovementComponent.h"

#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/CORE/MR_General.h"

UUnitMovementComponent::UUnitMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UUnitMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	MilitaryBase = Cast<AMilitaryBase>(GetOwner()); 
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUnitMovementComponent::SetTargetLoc()
{
	if (MilitaryBase)
	{
		// Zjistit target loc ze základny která není moje
		// TargetLoc = MilitaryBase->GetSpawnpointGround().GetLocation();
	}
}
