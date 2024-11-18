#include "UnitMovementComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/CORE/PC_MR_General.h"
#include "GameFramework/GameState.h"
#include "Net/UnrealNetwork.h"


UUnitMovementComponent::UUnitMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bAutoActivate = true;
}

void UUnitMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUnitMovementComponent, TargetLoc);
}

void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	SetTargetLoc();
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUnitMovementComponent::SetTargetLoc()
{
	if (UWorld* World = GetWorld())
	{
		APC_MR_General* PC = Cast<APC_MR_General>(World->GetGameInstance()->GetFirstLocalPlayerController()) ;
		if (PC && PC->bIsReferencesSet)
		{
			// Sem dojde
			if (AMR_General* OtherPawn = PC->OtherPlayerPawn)
			{
				DBG_5S("OtherPawn")
				TargetLoc = OtherPawn->BaseInstance->GetSpawnpointGround().GetLocation();
				TargetLoc = FVector(0.f, 0.f, 100.f);
			}
		}
	}
}
