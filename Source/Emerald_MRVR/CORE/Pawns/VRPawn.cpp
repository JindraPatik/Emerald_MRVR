#include "VRPawn.h"

#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/CORE/GameModes/GameModeMultiplayer.h"
#include "Components/StaticMeshComponent.h"
#include "Emerald_MRVR/Components/Health/HealthComponent.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"
#include "Net/UnrealNetwork.h"
#include "MotionControllerComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/Actors/MilitaryStation/Building.h"
#include "Emerald_MRVR/Components/MilitaryStation/MilitaryStationComp.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetMathLibrary.h"

AVRPawn::AVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// COMPONENTS
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	HealthComponent->SetIsReplicated(true);
	
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>("Resources");
	ResourcesComponent->SetIsReplicated(true);
	
	MilitaryStationComp = CreateDefaultSubobject<UMilitaryStationComp>("MilitaryBaseComp");
	MilitaryStationComp->SetIsReplicated(true);
	// ~COMPONENTS

}

void AVRPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AEKGameState* AEKGameStateInst = Cast<AEKGameState>(GetWorld()->GetGameState());
	if (!AEKGameStateInst)
	{
		return;
	}
	
	AGameModeCommon* GameModeCommon = Cast<AGameModeCommon>(AEKGameStateInst->AuthorityGameMode);
	if (!GameModeCommon)
	{
		return;
	}
	
	GameModeCommon->OnGameStartedDelegate.AddDynamic(this, &AVRPawn::StartGame);
	GameModeCommon->OnGameEndedDelegate.AddDynamic(this, &AVRPawn::EndGame);
}

// REPLICATED PROPS
void AVRPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AVRPawn, MilitaryStationInstance);
	DOREPLIFETIME(AVRPawn, MilitaryStationComp);
	DOREPLIFETIME(AVRPawn, SelectedBuildingActor);
	DOREPLIFETIME(AVRPawn, bInputIsEnabled);
}
// ~REPLICATED PROPS

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}


void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AGameModeMultiplayer>(GetWorld()->GetAuthGameMode());

	if (IsLocallyControlled())
	{
		MilitaryStationComp->SpawnMilitaryStation(this);
		MilitaryStationComp->SpawnBuildings(this);
	}
}

void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AVRPawn::TogglePlayerInputEnabled()
{
	bInputIsEnabled = !bInputIsEnabled;
}

void AVRPawn::MovePlayerOnCircle(AActor* Player, float InDelta, float& Angle, float Speed)
{
	if (!Player)
	{
		return;
	}

	FVector CurrentPosition = Player->GetActorLocation();

	float Distance = FVector(CurrentPosition.X, CurrentPosition.Y, 0.0f).Size();

	Angle = FMath::Atan2(CurrentPosition.Y, CurrentPosition.X);
	
	Angle += Speed * InDelta;

	if (Angle > 2 * PI)
	{
		Angle -= 2 * PI;
	}

	FVector NewPosition;
	NewPosition.X = Distance * FMath::Cos(Angle);
	NewPosition.Y = Distance * FMath::Sin(Angle);
	NewPosition.Z = CurrentPosition.Z; 

	Player->SetActorLocation(NewPosition);

	FVector LookAtPosition(0.0f, 0.0f, CurrentPosition.Z); 
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(NewPosition, LookAtPosition);
	Player->SetActorRotation(LookAtRotation);
}

void AVRPawn::MovePlayerOnRadius(AVRPawn* VRPawn, float InDelta, float& Distance, float Speed)
{
	if (!VRPawn)
	{
		return;
	}

	FVector CurrentPosition = VRPawn->GetActorLocation();

	float Angle = FMath::Atan2(CurrentPosition.Y, CurrentPosition.X);

	if (Distance <= 0.0f)
	{
		Distance = FVector(VRPawn->GetActorLocation().X, VRPawn->GetActorLocation().Y, 0.0f).Size();
	}

	Distance += Speed * InDelta;
	Distance = FMath::Clamp(Distance, 10.f, 8000.f);

	FVector NewPosition;
	NewPosition.X = Distance * FMath::Cos(Angle);
	NewPosition.Y = Distance * FMath::Sin(Angle);
	NewPosition.Z = CurrentPosition.Z;

	VRPawn->SetActorLocation(NewPosition);

	FVector LookAtPosition(0.0f, 0.0f, CurrentPosition.Z);
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(NewPosition, LookAtPosition);
	VRPawn->SetActorRotation(LookAtRotation);
}


void AVRPawn::StartGame()
{
	TogglePlayerInputEnabled();
	ResourcesComponent->StartGrowResources();
}

void AVRPawn::EndGame(APawn* Looser)
{
	TogglePlayerInputEnabled();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = FVector(0.f, 0.f, 200.f);
	FRotator Rotation = FRotator::ZeroRotator;
	if (!EndGameWidgetActor)
	{
		return;
	}
	
	AActor* EndGameActorInst = GetWorld()->SpawnActor<AActor>(EndGameWidgetActor, Location, Rotation, SpawnParameters);
	bIsMenuActive = true;

	if (!EndGameActorInst)
	{
		return;
	}
	
	UWidgetComponent* EndGameWidgetInst = EndGameActorInst->FindComponentByClass<UWidgetComponent>();
	if (!EndGameWidgetInst)
	{
		return;
	}
	
	UTextBlock* TXT_CountDown = Cast<UTextBlock>(EndGameWidgetInst->GetWidget()->WidgetTree->FindWidget("TXT_Condition"));
	if (Looser == Cast<APawn>(this))
	{
		if (!TXT_CountDown)
		{
			return;
		}
		
		TXT_CountDown->SetText(FText::FromString("You LOOSE!"));
	}
	else
	{
		if (!TXT_CountDown)
		{
			return;
		}
		
		TXT_CountDown->SetText(FText::FromString("You WON!"));
	}
}


void AVRPawn::SpawnPreviewUnit(ABuilding* BuildingActor)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLoc = MotionController_R->GetComponentLocation() + FVector(0.f,0.f,15.f);
	FRotator SpawnRot = FRotator::ZeroRotator;
	
	PreviewInstance = World->SpawnActor<AActor>(PreviewUnitClass, SpawnLoc, SpawnRot, SpawnParameters);
	UStaticMeshComponent* PreviewMeshComponent = PreviewInstance->FindComponentByClass<UStaticMeshComponent>();
	PreviewMeshComponent->AttachToComponent(MotionController_R, FAttachmentTransformRules::KeepRelativeTransform);
	
	if (!PreviewMeshComponent)
	{
		return;
	}
	
	UStaticMesh* PreviewBody = BuildingActor->BuildingDataAsset->UnitToSpawnData->UnitBody;
	if (!PreviewBody)
	{
		return;
	}
	
	PreviewMeshComponent->SetStaticMesh(PreviewBody);
}

void AVRPawn::OnSelectedModuleChanged()
{
	if (PreviewInstance)
	{
		PreviewInstance->Destroy();
	}
	
	SpawnPreviewUnit(SelectedBuildingActor);
}

void AVRPawn::AddPowerUp(APowerUp* InPowerUp)
{
	AvailablePowerUps.Add(InPowerUp);
	UE_LOG(LogTemp, Warning, TEXT("AVRPawn::AddPowerUp added"));
}







