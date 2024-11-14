#include "EK_GameMode.h"

#include "EngineUtils.h"
#include "MR_General.h"
#include "PC_MR_General.h"
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

	AllPCs.Add(NewPlayer);
}

void AEK_GameMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);

	AllPCs.Add(NewPC);
}

void AEK_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AllPCs.Remove(Cast<APC_MR_General>(Exiting));
}

// Begin Play
void AEK_GameMode::BeginPlay()
{
	Super::BeginPlay();
	FindAllPlayerStarts();
}

// REPLICATION
void AEK_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Iterate all Player starts and return FTransform
FTransform AEK_GameMode::FindMyPlayerStart()
{
	FTransform Transform;
	if (AllPlayerStarts.Num() < 0) { return Transform; }

	APlayerStart* SelectedPlayerStart = AllPlayerStarts[0];
	AllPlayerStarts.Remove(SelectedPlayerStart);
	return SelectedPlayerStart->GetTransform();

}

// Spawn player at custom player start
void AEK_GameMode::SpawnPlayer(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		FActorSpawnParameters PawnSpawnParameters;
		
		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn)
		{
			Pawn->Destroy();
			PlayerPawn = GetWorld()->SpawnActor<AMR_General>(PawnToSpawn, FindMyPlayerStart(), PawnSpawnParameters);
			PlayerController->Possess(PlayerPawn);
		}
		else
		{
			PlayerPawn = GetWorld()->SpawnActor<AMR_General>(PawnToSpawn, FindMyPlayerStart(), PawnSpawnParameters);
			PlayerController->Possess(PlayerPawn);
		}
		PlayerPawn->PossessedBy(PlayerController);
		
	}	
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

void AEK_GameMode::FindAllPlayerStarts()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		AllPlayerStarts.Add(*It);
	}
}







