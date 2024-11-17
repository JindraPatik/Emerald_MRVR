#include "UnitMovementComponent.h"

#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/CORE/PC_MR_General.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"


UUnitMovementComponent::UUnitMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	SetTargetLoc();
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
	// SetTargetLoc();
}

void UUnitMovementComponent::SetTargetLoc()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APC_MR_General* PC = Cast<APC_MR_General>(World->GetGameInstance()->GetFirstLocalPlayerController()) ;
		if (PC)
		{
			AMR_General* OtherPawn = PC->GetOtherPlayerPawn();
			if (OtherPawn)
			{
				TargetLoc = OtherPawn->BaseInstance->GetSpawnpointGround().GetLocation();
				// UE_LOG() doplnit
			}
		}
	}
	
	
			
	
}
