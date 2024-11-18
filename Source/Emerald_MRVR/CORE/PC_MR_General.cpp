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


void APC_MR_General::SetOtherPlayerPC()
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
        					OtherPlayerPC = PlayerControler;
        				}
        			}
        		}
        	}
}

void APC_MR_General::SetOtherPlayerPawn()
{
	/*AMR_General* OtherPawn = Cast<AMR_General>(OtherPlayerPC->GetPawn());
	if (OtherPawn)
	{
		OtherPlayerPawn = OtherPawn;
	}
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(2, 20.f, FColor::Green, FString::Printf(TEXT("No OtherPlayer")));*/
	if (OtherPlayerPC)
	{
		AMR_General* OtherPawn = Cast<AMR_General>(OtherPlayerPC->GetPawn());
		if (OtherPawn)
		{
			OtherPlayerPawn = OtherPawn;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("OtherPlayerPC does not have a Pawn"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("OtherPlayerPC is nullptr"));
	}
}

void APC_MR_General::Server_SpawnPlayer_Implementation()
{
	AEK_GameMode* GameMode = Cast<AEK_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->SpawnPlayer(this);
	}
}
