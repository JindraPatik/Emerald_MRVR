// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_MR_General.h"

#include "EK_GameMode.h"
#include "MR_General.h"
#include "Emerald_MRVR/Widgets/ResourcesWidget.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Net/UnrealNetwork.h"


void APC_MR_General::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	
	if (IsLocalController())
	{
		Server_SpawnPlayer();
	}
}

void APC_MR_General::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APC_MR_General, OtherPlayerPC);
}

void APC_MR_General::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AMR_General* General = Cast<AMR_General>(InPawn);
	if (General)
	{
		General->SpawnMilitaryBase();
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


