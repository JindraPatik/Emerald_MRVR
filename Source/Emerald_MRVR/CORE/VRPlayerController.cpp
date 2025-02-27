#include "VRPlayerController.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"
#include "Net/UnrealNetwork.h"


void AVRPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AVRPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVRPlayerController, OtherPlayerPC);
}

void AVRPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	VR_Pawn = Cast<AVRPawn>(InPawn);
	if (!VR_Pawn)
	{
		return;
	}
	
	if (IsLocalController())
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
	}
	
	VR_Pawn->bPossesed = true;
}




