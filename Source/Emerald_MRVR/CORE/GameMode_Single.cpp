#include "GameMode_Single.h"
#include "AIPawn.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "VRPawn.h"
#include "Emerald_MRVR/Components/CrystalSpawnerComp.h"


AGameMode_Single::AGameMode_Single()
{
	CrystalSpawner = CreateDefaultSubobject<UCrystalSpawnerComp>("CrystalSpawner");
	Mr_General_Class = AVRPawn::StaticClass();
}

void AGameMode_Single::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	FindAllPlayerStarts();
}

void AGameMode_Single::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemyAI();
	StartCountdown();
}

void AGameMode_Single::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	SpawnPlayer(NewPlayer);
}

void AGameMode_Single::StartGame()
{
	Super::StartGame();
	if (CrystalSpawner)
	{
		CrystalSpawner->StartSpawning();
	}
	
}

void AGameMode_Single::FindAllPlayerStarts()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		if (IsValid(PlayerStart))
		{
			AllPlayerStarts.Add(PlayerStart);
		}
	}

	if (AllPlayerStarts.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerStart found in the world!"));
	}
}

// Iterate all Player starts and return FTransform
FTransform AGameMode_Single::FindMyPlayerStart()
{
	if (AllPlayerStarts.Num() == 0) 
	{
		UE_LOG(LogTemp, Error, TEXT("No available PlayerStart!"));
		return FTransform(); // Invalid Transform
	}

	APlayerStart* SelectedPlayerStart = AllPlayerStarts[0];
	AllPlayerStarts.Remove(SelectedPlayerStart);
	SelectedPlayerStart->Destroy();

	FTransform PlayerStartTransform = SelectedPlayerStart->GetActorTransform();
	if (!PlayerStartTransform.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerStart Transform is invalid!"));
	}
	return PlayerStartTransform;
}


void AGameMode_Single::SpawnPlayer(APlayerController* PlayerController)
{
	if (!PlayerController) return;

	FTransform PlayerStartTransform = FindMyPlayerStart();
	if (!PlayerStartTransform.IsValid()) return;

	FVector Location = PlayerStartTransform.GetLocation();
	FRotator Rotation = PlayerStartTransform.GetRotation().Rotator();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerController;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (APawn* ExistingPawn = PlayerController->GetPawn())
	{
		ExistingPawn->Destroy();
	}

	// Spawn spectator player?
	UClass* PawnToSpawn = bIsSpectator ? SpectatorPawn : Mr_General_Class;

	AVRPawn* NewPawn = GetWorld()->SpawnActor<AVRPawn>(PawnToSpawn, Location, Rotation, SpawnParams);
	if (NewPawn)
	{
		PlayerController->Possess(NewPawn);
	}
}

void AGameMode_Single::SpawnEnemyAI()
{
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	EnemyPawn = GetWorld()->SpawnActor<AAIPawn>(EnemyToSpawn, Location, Rotation, SpawnParams);
}



