#include "EK_GameMode.h"

#include "EngineUtils.h"
#include "MR_General.h"
#include "Emerald_MRVR/Components/CrystalSpawnerComp.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "Net/UnrealNetwork.h"

AEK_GameMode::AEK_GameMode()
{
	CrystalSpawner = CreateDefaultSubobject<UCrystalSpawnerComp>("CrystalSpawner");
	PawnToSpawn = AMR_General::StaticClass();
}

void AEK_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (PlayerStarts.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough player starts"));
		return;
	}

	int32 PlayerIndex = GameState->PlayerArray.Num() - 1;
	AActor* SelectedPlayerStart = (PlayerIndex == 0) ? PlayerStarts[0] : PlayerStarts[1];
	
	
	if (SelectedPlayerStart)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = NewPlayer;

		FVector SpawnLoc = SelectedPlayerStart->GetActorLocation();
		FRotator SpawnRot = SelectedPlayerStart->GetActorRotation();

		AMR_General* NewPlayerPawn = GetWorld()->SpawnActor<AMR_General>(PawnToSpawn, SpawnLoc, SpawnRot);

		if (NewPlayerPawn)
		{
			NewPlayer->Possess(NewPlayerPawn);
		}
	}
}

void AEK_GameMode::BeginPlay()
{
	Super::BeginPlay();

	if (CrystalSpawner)
	{
		CrystalSpawner->StartSpawning();
	}

	GetAllPlayerStarts();
	
}


void AEK_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEK_GameMode, TargetPoints);
}



TArray<ATargetPoint*> AEK_GameMode::GetAllTargetpoints()
{
	TArray<ATargetPoint*> TPs;
	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		TargetPoints.Add(*It);
	}
	return TPs;
}

void AEK_GameMode::GetAllPlayerStarts()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		PlayerStarts.Add(*It);
	}
}




