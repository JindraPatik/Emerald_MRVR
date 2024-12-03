// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_MR_General.h"

#include "MR_General.h"
#include "Net/UnrealNetwork.h"


void APC_MR_General::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
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
