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
	/*UWorld* World = GetWorld();
	if (World)
	{
		APC_MR_General* PC_MR_General = Cast<APC_MR_General>(World->GetGameInstance()->GetFirstLocalPlayerController());
		if (PC_MR_General)
		{
			APC_MR_General* Other_PC = PC_MR_General->GetOtherPlayerPC();
			if (Other_PC)
			{
				AMR_General* OtherPawn = Cast<AMR_General>(Other_PC->GetPawn());
				if (OtherPawn)
				{
					TargetLoc = OtherPawn->BaseInstance->GetSpawnpointGround().GetLocation();
				}
			}
		}
	}*/
	
}
