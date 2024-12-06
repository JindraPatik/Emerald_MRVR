// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_MR_General.h"
#include "MR_General.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/Components/MilitaryBaseComp.h"
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
		DBG_ONE_PARAM(10, "PC: %s", *this->GetName());

		if (IsLocalController()) // Pouze lokálně vlastněný hráč zpracovává vstupy
		{
			EnableInput(this);
			QQQ("Local input enabled")
			SpawnBaseAndModules();
		}
		else
		{
			DisableInput(this);
			QQQ("Disabled Input")
		}
	General->bPossesed = true;
	}
}

void APC_MR_General::SpawnBaseAndModules()
{
	if (!HasAuthority())
	{
		Server_SpawnBaseAndModules();
		return;
	}
	//General->MilitaryBaseComp->SpawnMilitaryBase(General);
	//General->MilitaryBaseComp->SpawnModules(General);
}

void APC_MR_General::Server_SpawnBaseAndModules_Implementation()
{
	SpawnBaseAndModules();
}



