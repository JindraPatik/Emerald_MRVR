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
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->AddMappingContext(GameplayInputMappingContext, 1);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	// Bindings
	Input->BindAction(IA_SpawnUnit, ETriggerEvent::Started, this, &AVRPawn::Action_SpawnUnit);
	Input->BindAction(IA_SelectBuilding_L, ETriggerEvent::Started, this, &AVRPawn::SelectBuilding_L);
	Input->BindAction(IA_SelectBuilding_R, ETriggerEvent::Started, this, &AVRPawn::SelectBuilding_R);
}
// ~PLAYER INPUT


// BEGIN PLAY
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AMultiplayer_GameMode>(GetWorld()->GetAuthGameMode());

	PointerStick_L->SetVisibility(false);
	PointerStick_R->SetVisibility(false);
	
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

	if (IsLocallyControlled() && bInputIsEnabled)
	{
		if (!bIsMenuActive)
		{
			if (MotionController_L && ImpactPointer_L)
			{
				PerformSphereTrace(MotionController_L, ImpactPointer_L, CurrentlyHoveredBuilding_L);
			}
			if (MotionController_R && ImpactPointer_R)
			{
				PerformSphereTrace(MotionController_R, ImpactPointer_R, CurrentlyHoveredBuilding_R);
			}
		}
	}
}
// ~TICK

void AVRPawn::PerformSphereTrace(
	UMotionControllerComponent* UsedController,
	UStaticMeshComponent* ImpactPointer,
	ABuilding*& CurrentlyHoveredBuilding)
	{
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FVector Start = UsedController->GetComponentLocation();
		FVector End = Start + (UsedController->GetForwardVector() * 3000.f);
		float Radius = 5.f;

		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			Start,
			End,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(Radius),
			QueryParams);

		if (bHit)
		{
			if (ImpactPointer)
			{
				ImpactPointer->SetWorldLocation(HitResult.Location);
			}

			ABuilding* HitModule = Cast<ABuilding>(HitResult.GetActor());
			AVRPawn* HittedPawn = Cast<AVRPawn>(HitResult.GetActor()->GetOwner());

			if (HitModule && HittedPawn == this)
			{
				CurrentlyHoveredBuilding = HitModule;
				ImpactPointer->SetWorldLocation(CurrentlyHoveredBuilding->GetActorLocation() + FVector(0.f, 0.f, 8.f));
				PrevisouslyHighlitedBuilding = HitModule;
				CurrentlyHoveredBuilding->EnableInfoWidget();
				CurrentlyHoveredBuilding->SetOverlayMaterial();
				if (CurrentlyHoveredBuilding != PrevisouslyHighlitedBuilding)
				{
					PrevisouslyHighlitedBuilding->DisableInfoWidget();
					PrevisouslyHighlitedBuilding->RemoveOverlayMaterial();
				}
			}
			else
			{
				if (CurrentlyHoveredBuilding)
				{
					CurrentlyHoveredBuilding->DisableInfoWidget();
					CurrentlyHoveredBuilding = nullptr;
					PrevisouslyHighlitedBuilding->DisableInfoWidget();
					PrevisouslyHighlitedBuilding->RemoveOverlayMaterial();
					
				}
			}
		}
	
		else
		{
			if (CurrentlyHoveredBuilding)
			{
				CurrentlyHoveredBuilding->DisableInfoWidget();
				CurrentlyHoveredBuilding->RemoveOverlayMaterial();
				CurrentlyHoveredBuilding = nullptr;
				PrevisouslyHighlitedBuilding->DisableInfoWidget();
				PrevisouslyHighlitedBuilding->RemoveOverlayMaterial();
				if (PrevisouslyHighlitedBuilding)
				{
					PrevisouslyHighlitedBuilding->DisableInfoWidget();
					PrevisouslyHighlitedBuilding->RemoveOverlayMaterial();
				}
			}

			if (ImpactPointer)
			{
				ImpactPointer->SetWorldLocation(End);
				if (PrevisouslyHighlitedBuilding)
				{
					PrevisouslyHighlitedBuilding->DisableInfoWidget();
					PrevisouslyHighlitedBuilding->RemoveOverlayMaterial();
				}
			}
			if (PrevisouslyHighlitedBuilding)
			{
				PrevisouslyHighlitedBuilding->DisableInfoWidget(); // upr
				PrevisouslyHighlitedBuilding->RemoveOverlayMaterial(); // upr
			}
		}
	}

void AVRPawn::EnablePlayerInput()
{
	bInputIsEnabled = !bInputIsEnabled;
	if (!bInputIsEnabled)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(MenuInputMappingContext, 0);
	}
	else
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(GameplayInputMappingContext, 1);
	}
}

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
	EnablePlayerInput();
	PointerStick_L->SetVisibility(true);
	PointerStick_R->SetVisibility(true);
	ResourcesComponent->StartGrowResources();
}

void AVRPawn::EndGame(APawn* Looser)
{
	EnablePlayerInput();

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








