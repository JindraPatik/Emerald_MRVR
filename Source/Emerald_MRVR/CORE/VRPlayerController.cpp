#include "VRPlayerController.h"
#include "VRPawn.h"
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
	General = Cast<AVRPawn>(InPawn);
	if (General)
	{
		if (IsLocalController())
		{
			EnableInput(this);
		}
		else
		{
			DisableInput(this);
		}
	General->bPossesed = true;
	}
}




