#include "Gamemode_Single.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/PlayerStart.h"
#include "MR_General.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/Components/CrystalSpawnerComp.h"



AGamemode_Single::AGamemode_Single()
{
	CrystalSpawner = CreateDefaultSubobject<UCrystalSpawnerComp>("CrystalSpawner");
	PawnToSpawn = AMR_General::StaticClass();
}

void AGamemode_Single::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GetAllTargetpoints();
	FindAllPlayerStarts();
}

void AGamemode_Single::BeginPlay()
{
	Super::BeginPlay();
	
	if (CrystalSpawner)
	{
		CrystalSpawner->StartSpawning();
	}

}

void AGamemode_Single::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	SpawnPlayer(NewPlayer);
}



void AGamemode_Single::FindAllPlayerStarts()
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
FTransform AGamemode_Single::FindMyPlayerStart()
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

TArray<ATargetPoint*> AGamemode_Single::GetAllTargetpoints()
{
	TArray<ATargetPoint*> TPs;
	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		TargetPoints.Add(*It);
	}
	return TPs;
}

void AGamemode_Single::SpawnPlayer(APlayerController* PlayerController)
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
		UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
	}
}
