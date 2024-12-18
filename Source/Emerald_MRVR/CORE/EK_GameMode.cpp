#include "EK_GameMode.h"

#include "EKGameState.h"
#include "EngineUtils.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MR_General.h"
#include "PC_MR_General.h"
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

void AEK_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FindAllPlayerStarts();
}

void AEK_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	

	AllPCs.Add(NewPlayer);

	// Spawn player Pawn při přihlášení
	SpawnPlayer(NewPlayer);
	
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

void AEK_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (CrystalSpawner)
	{
		CrystalSpawner->StartSpawning();
	}
}

void AEK_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEK_GameMode, AllPlayerStarts);
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
	if (!HasAuthority() || !PlayerController) return;
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

	AMR_General* NewPawn = GetWorld()->SpawnActor<AMR_General>(PawnToSpawn, Location, Rotation, SpawnParams);
	if (NewPawn)
	{
		NewPawn->SetReplicates(true);
		PlayerController->Possess(NewPawn);
		// UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
	}
}

void AEK_GameMode::FindAllPlayerStarts()
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








