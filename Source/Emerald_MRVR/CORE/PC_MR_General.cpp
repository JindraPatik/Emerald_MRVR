#include "PC_MR_General.h"
#include "MR_General.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Net/UnrealNetwork.h"


void APC_MR_General::BeginPlay()
{
	Super::BeginPlay();
}

void APC_MR_General::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APC_MR_General, OtherPlayerPC);
}

void APC_MR_General::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	General = Cast<AMR_General>(InPawn);
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




