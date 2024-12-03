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
#include "Emerald_MRVR/DebugMacros.h"


AEK_GameMode::AEK_GameMode()
{
	CrystalSpawner = CreateDefaultSubobject<UCrystalSpawnerComp>("CrystalSpawner");
	PawnToSpawn = AMR_General::StaticClass();
}

void AEK_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AllPCs.Add(NewPlayer);
	GetAllTargetpoints();

	APC_MR_General* NewPC = Cast<APC_MR_General>(NewPlayer);
	if (!NewPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PostLogin: New PlayerController is not of type APC_MR_General."));
		return;
	}

	// Iterate over all PCs
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APC_MR_General* ExistingPC = Cast<APC_MR_General>(It->Get());
		if (ExistingPC && ExistingPC != NewPC) // Najdeme jiného hráče
		{
			// Set references to others PCs
			NewPC->OtherPlayerPC = ExistingPC;
			ExistingPC->OtherPlayerPC = NewPC;
		}
	}
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
	CrystalSpawner->StartSpawning();
}

// REPLICATION
void AEK_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(AEK_GameMode, TargetPoints);
}

// Iterate all Player starts and return FTransform
FTransform AEK_GameMode::FindMyPlayerStart()
{
	if (AllPlayerStarts.Num() == 0) 
	{
		UE_LOG(LogTemp, Error, TEXT("No available PlayerStart!"));
		return FTransform(); // Invalid Transform
	}

	APlayerStart* SelectedPlayerStart = AllPlayerStarts[0];
	AllPlayerStarts.Remove(SelectedPlayerStart);

	FTransform PlayerStartTransform = SelectedPlayerStart->GetActorTransform();
	if (!PlayerStartTransform.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerStart Transform is invalid!"));
	}

	return PlayerStartTransform;
}

// Spawn player at custom player start
void AEK_GameMode::SpawnPlayer(APlayerController* PlayerController)
{
	if (!PlayerController) return;

	FTransform PlayerStartTransform = FindMyPlayerStart();
	if (!PlayerStartTransform.IsValid()) return;

	FVector Location = PlayerStartTransform.GetLocation();
	FRotator Rotation = PlayerStartTransform.Rotator();

	UE_LOG(LogTemp, Log, TEXT("Spawning player at Location: %s, Rotation: %s"),
		*Location.ToString(), *Rotation.ToString());

	FActorSpawnParameters PawnSpawnParameters;
	PawnSpawnParameters.Owner = PlayerController;
	PawnSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (PlayerController->GetPawn())
	{
		PlayerController->GetPawn()->Destroy();
	}

	PlayerPawn = GetWorld()->SpawnActor<AMR_General>(PawnToSpawn, Location, Rotation, PawnSpawnParameters);
	if (PlayerPawn)
	{
		PlayerPawn->SetReplicates(true); // Zajistí replikaci Pawna
		PlayerController->Possess(PlayerPawn);
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
	AllPlayerStarts.Empty(); // Reset pole při nové iteraci

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







