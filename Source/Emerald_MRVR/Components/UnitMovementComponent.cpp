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
	DOREPLIFETIME(UUnitMovementComponent, TargetLoc);
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
		if (GetOwner()->HasAuthority())
		{
			Server_MoveTo(TargetLoc);
		}
		else
		{
			Server_MoveTo(TargetLoc);
		}
	}
}

void UUnitMovementComponent::Server_SetTargetLoc_Implementation()
{
	Multi_SetTargetLoc();
}

void UUnitMovementComponent::Multi_SetTargetLoc_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APC_MR_General* MyPC = Cast<APC_MR_General>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController());
		if (MyPC)
		{
			AMR_General* General = Cast<AMR_General>(MyPC->GetPawn());
			if (General)
			{
				TargetLoc = General->BaseInstance->GetSpawnpointGround().GetLocation();
			}
		}
	}
}

void UUnitMovementComponent::Server_MoveTo_Implementation(FVector TargetLocation) const
{
	Multi_MoveTo(TargetLocation);
}

void UUnitMovementComponent::Multi_MoveTo_Implementation(FVector TargetLocation) const
{
	AUnit* Unit = Cast<AUnit>(GetOwner());
	if (!Unit)
	{
		return;
	}
	FVector CurrentLoc =  Unit->GetActorLocation();
	FVector Direction = (TargetLocation - CurrentLoc).GetSafeNormal();
	float Speed = Unit->Speed;
	FVector NewLocation = CurrentLoc + Direction * Speed;
	Unit->SetActorLocation(NewLocation);
}

