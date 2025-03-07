#include "GameModeSingle.h"
#include "EngineUtils.h"
#include "Emerald_MRVR/Components/Resources/CrystalSpawnerComp.h"
#include "Emerald_MRVR/Components/Resources/PowerUpSpawner.h"
#include "Emerald_MRVR/CORE/Pawns/AIPawn.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"
#include "GameFramework/PlayerStart.h"

AGameModeSingle::AGameModeSingle()
{
	CrystalSpawner = CreateDefaultSubobject<UCrystalSpawnerComp>("CrystalSpawner");
	PowerUpSpawner = CreateDefaultSubobject<UPowerUpSpawner>("PowerUpSpawner");
	VRPawnClass = AVRPawn::StaticClass();
}

void AGameModeSingle::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	FindAllPlayerStarts();
}

void AGameModeSingle::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnEnemyAI();
	StartCountdown();
}

void AGameModeSingle::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	SpawnPlayer(NewPlayer);
}

void AGameModeSingle::StartGame()
{
	Super::StartGame();
	
	if (CrystalSpawner)
	{
		CrystalSpawner->StartSpawning();
	}
	
}

void AGameModeSingle::FindAllPlayerStarts()
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
FTransform AGameModeSingle::FindMyPlayerStart()
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


void AGameModeSingle::SpawnPlayer(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return;
	}
	
	FTransform PlayerStartTransform = FindMyPlayerStart();
	if (!PlayerStartTransform.IsValid())
	{
		return;
	}
	
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
	UClass* PawnToSpawn = bIsSpectator ? SpectatorPawn : VRPawnClass;

	AVRPawn* VRPawn = GetWorld()->SpawnActor<AVRPawn>(PawnToSpawn, Location, Rotation, SpawnParams);
	if (VRPawn)
	{
		PlayerController->Possess(VRPawn);
	}
}

void AGameModeSingle::SpawnEnemyAI()
{
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	EnemyPawn = GetWorld()->SpawnActor<AAIPawn>(EnemyToSpawn, Location, Rotation, SpawnParams);
}



