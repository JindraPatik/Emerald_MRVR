// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_MR_General.h"

#include "EK_GameMode.h"
#include "MR_General.h"
#include "Emerald_MRVR/Widgets/ResourcesWidget.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Emerald_MRVR/DebugMacros.h"


void APC_MR_General::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		Server_SpawnPlayer();
	}
}

APC_MR_General* APC_MR_General::GetOtherPlayerPC() const
{
	UWorld* World = GetWorld();
        	if (World)
        	{
        		AGameStateBase* GameState = World->GetGameState();
        		if (GameState)
        		{
        			for (APlayerState* PlayerStateInst : GameState->PlayerArray)
        			{
        				APC_MR_General* PlayerControler = Cast<APC_MR_General>(PlayerStateInst->GetOwner());
        				if (PlayerControler && PlayerControler != this)
        				{
        					APC_MR_General* OtherPlayerPC = PlayerControler;
        					return OtherPlayerPC;
        				}
        			}
        		}
        	}
	return nullptr;
}

AMR_General* APC_MR_General::GetOtherPlayerPawn() const
{
	AMR_General* OtherPawn = Cast<AMR_General>(GetOtherPlayerPC()->GetPawn());
	if (OtherPawn)
	{
		return OtherPawn;
	}
	return nullptr;
}

void APC_MR_General::Server_SpawnPlayer_Implementation()
{
	AEK_GameMode* GameMode = Cast<AEK_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->SpawnPlayer(this);
	}
}
