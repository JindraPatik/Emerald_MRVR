#include "VRPawn.h"

#include "Emerald_MRVR/CORE/EKGameState.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Emerald_MRVR/CORE/GameModes/Multiplayer_GameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Emerald_MRVR/Components/Health/HealthComponent.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"
#include "Net/UnrealNetwork.h"
#include "MotionControllerComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/Actors/MilitaryBase/Building.h"
#include "Emerald_MRVR/Components/MilitaryBase/MilitaryStationComp.h"
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
	if (AEKGameStateInst)
	{
		AGameModeCommon* GameModeCommon = Cast<AGameModeCommon>(AEKGameStateInst->AuthorityGameMode);
		if (GameModeCommon)
		{
			GameModeCommon->OnGameStartedDelegate.AddDynamic(this, &AVRPawn::StartGame);
			GameModeCommon->OnGameEndedDelegate.AddDynamic(this, &AVRPawn::EndGame);
		}
	}
}

// REPLICATED PROPS
void AVRPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AVRPawn, MilitaryStationInstance);
	DOREPLIFETIME(AVRPawn, PlayerDefaultColor);
	DOREPLIFETIME(AVRPawn, CurrentlyHoveredBuilding_L);
	DOREPLIFETIME(AVRPawn, CurrentlyHoveredBuilding_R);
	DOREPLIFETIME(AVRPawn, MilitaryStationComp);
	DOREPLIFETIME(AVRPawn, SelectedBuildingActor);
	DOREPLIFETIME(AVRPawn, bInputIsEnabled);
}
// ~REPLICATED PROPS


// PLAYER INPUT
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	}
// ~PLAYER INPUT


// BEGIN PLAY
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AMultiplayer_GameMode>(GetWorld()->GetAuthGameMode());


	// SetPlayerColor();

	if (IsLocallyControlled())
	{
		MilitaryStationComp->SpawnMilitaryStation(this);
		MilitaryStationComp->SpawnBuildings(this);
	}
}
// ~BEGIN PLAY

// TICK
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// ~TICK



void AVRPawn::MovePlayerOnCircle(AActor* Player, float InDelta, float& Angle, float Speed)
{
	if (!Player) return;

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

void AVRPawn::StartGame()
{
	ResourcesComponent->StartGrowResources();
}

void AVRPawn::EndGame(APawn* Looser)
{

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = FVector(0.f, 0.f, 200.f);
	FRotator Rotation = FRotator::ZeroRotator;
	if (EndGameWidgetActor)
	{
		AActor* EndGameActorInst = GetWorld()->SpawnActor<AActor>(EndGameWidgetActor, Location, Rotation, SpawnParameters);
		bIsMenuActive = true;

		if (EndGameActorInst)
		{
			UWidgetComponent* EndGameWidgetInst = EndGameActorInst->FindComponentByClass<UWidgetComponent>();
			if (EndGameWidgetInst)
			{
				UTextBlock* TXT_CountDown = Cast<UTextBlock>(EndGameWidgetInst->GetWidget()->WidgetTree->FindWidget("TXT_Condition"));
				if (Looser == Cast<APawn>(this))
				{
					if (TXT_CountDown)
					{
						TXT_CountDown->SetText(FText::FromString("You LOOSE!"));
					}
				}
				else
				{
					if (TXT_CountDown)
					{
						TXT_CountDown->SetText(FText::FromString("You WON!"));
					}
				}
			}
		}
	}
}

void AVRPawn::SelectBuilding_L()
{
	if (CurrentlyHoveredBuilding_L)
	{
		SelectedBuildingActor = CurrentlyHoveredBuilding_L;
		OnSelectedModuleChanged();
	}
}

void AVRPawn::SelectBuilding_R()
{
	if (CurrentlyHoveredBuilding_R)
	{
		SelectedBuildingActor = CurrentlyHoveredBuilding_R;
		OnSelectedModuleChanged();
	}
}

void AVRPawn::Action_SpawnUnit()
{
	AGameModeCommon* GM = Cast<AGameModeCommon>(GetWorld()->GetAuthGameMode());
	if (GM && GM->bGameHasStarted)
	{
		if (IsLocallyControlled())
		{
			MilitaryStationComp->SpawnUnit(this, SelectedBuildingActor);
		}
		
	}
}

void AVRPawn::SpawnPreviewUnit(ABuilding* BuildingActor)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SpawnLoc = MotionController_R->GetComponentLocation() + FVector(0.f,0.f,15.f);
		FRotator SpawnRot = FRotator::ZeroRotator;
		PreviewInstance = World->SpawnActor<AActor>(PreviewUnitClass, SpawnLoc, SpawnRot, SpawnParameters);
		UStaticMeshComponent* PreviewMeshComponent = PreviewInstance->FindComponentByClass<UStaticMeshComponent>();
		PreviewMeshComponent->AttachToComponent(MotionController_R, FAttachmentTransformRules::KeepRelativeTransform);
		if (PreviewMeshComponent)
		{
			UStaticMesh* PreviewBody = BuildingActor->BuildingDataAsset->UnitToSpawnData->UnitBody;
			if (PreviewBody)
			{
				PreviewMeshComponent->SetStaticMesh(PreviewBody);
			}
		}
	}
}

void AVRPawn::OnSelectedModuleChanged()
{
	if(PreviewInstance)
	{
		PreviewInstance->Destroy();
	}
	SpawnPreviewUnit(SelectedBuildingActor);
}








