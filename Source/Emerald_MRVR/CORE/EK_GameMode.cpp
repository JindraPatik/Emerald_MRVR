#include "EK_GameMode.h"

#include "EngineUtils.h"
#include "MR_General.h"
#include "Emerald_MRVR/Components/CrystalSpawnerComp.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AEK_GameMode::AEK_GameMode()
{
	CrystalSpawner = CreateDefaultSubobject<UCrystalSpawnerComp>("CrystalSpawner");
	PawnToSpawn = AMR_General::StaticClass();
}

void AEK_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}


void AEK_GameMode::BeginPlay()
{
	Super::BeginPlay();
	FindedPlayerStarts = GetAllPlayerStarts();

	if (CrystalSpawner)
	{
		CrystalSpawner->StartSpawning();
	}
}

void AEK_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEK_GameMode, TargetPoints);
	DOREPLIFETIME(AEK_GameMode, FindedPlayerStarts);
}

AActor* AEK_GameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (FindedPlayerStarts.Num() > 1)
	{
		AActor* SelectedPlayerStart = FindedPlayerStarts.Pop();
		return SelectedPlayerStart;
	}
	return Super::ChoosePlayerStart_Implementation(Player);
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

TArray<AActor*> AEK_GameMode::GetAllPlayerStarts()
{
	TArray<AActor*> PlayerStarts;
    
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		PlayerStarts.Add(*It);
	}

	return PlayerStarts;
}





