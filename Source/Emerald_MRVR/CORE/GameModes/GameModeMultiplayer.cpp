#include "GameModeMultiplayer.h"

#include "EngineUtils.h"
#include "Emerald_MRVR/Components/Resources/CrystalSpawnerComp.h"
#include "Emerald_MRVR/CORE/VRPlayerController.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "Net/UnrealNetwork.h"


AGameModeMultiplayer::AGameModeMultiplayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CrystalSpawner = ObjectInitializer.CreateDefaultSubobject<UCrystalSpawnerComp>(this,"CrystalSpawner");
	PawnToSpawn = AVRPawn::StaticClass();
}

void AGameModeMultiplayer::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FindAllPlayerStarts();
}

void AGameModeMultiplayer::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	

	AllPCs.Add(NewPlayer);

	// Spawn player Pawn při přihlášení
	SpawnPlayer(NewPlayer);
	
}

void AGameModeMultiplayer::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);

	AllPCs.Add(NewPC);
}

void AGameModeMultiplayer::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AllPCs.Remove(Cast<AVRPlayerController>(Exiting));
}

void AGameModeMultiplayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (CrystalSpawner)
	{
		CrystalSpawner->StartSpawning();
	}
}

void AGameModeMultiplayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameModeMultiplayer, AllPlayerStarts);
}

// Iterate all Player starts and return FTransform
FTransform AGameModeMultiplayer::FindMyPlayerStart()
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
void AGameModeMultiplayer::SpawnPlayer(APlayerController* PlayerController)
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

	AVRPawn* VR_Pawn = GetWorld()->SpawnActor<AVRPawn>(PawnToSpawn, Location, Rotation, SpawnParams);
	if (VR_Pawn)
	{
		VR_Pawn->SetReplicates(true);
		PlayerController->Possess(VR_Pawn);
		// UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
	}
}

void AGameModeMultiplayer::FindAllPlayerStarts()
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








