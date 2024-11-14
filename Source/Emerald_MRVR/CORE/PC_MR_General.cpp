// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_MR_General.h"

#include "EK_GameMode.h"
#include "Emerald_MRVR/Widgets/ResourcesWidget.h"
#include "Kismet/GameplayStatics.h"


void APC_MR_General::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		Server_SpawnPlayer();
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
