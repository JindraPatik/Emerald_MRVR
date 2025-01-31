#include "MR_General.h"

#include "EKGameState.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EK_GameMode.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"
#include "Net/UnrealNetwork.h"
#include "MotionControllerComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Camera/CameraComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/ModuleActor.h"
#include "Emerald_MRVR/Components/MilitaryBaseComp.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetMathLibrary.h"

AMR_General::AMR_General()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// COMPONENTS
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	HealthComponent->SetIsReplicated(true);
	
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>("Resources");
	ResourcesComponent->SetIsReplicated(true);
	
	MilitaryBaseComp = CreateDefaultSubobject<UMilitaryBaseComp>("MilitaryBaseComp");
	MilitaryBaseComp->SetIsReplicated(true);

	// ~COMPONENTS

}

void AMR_General::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AEKGameState* AEK_GameStateInst = Cast<AEKGameState>(GetWorld()->GetGameState());
	if (AEK_GameStateInst)
	{
		AGM* GM = Cast<AGM>(AEK_GameStateInst->AuthorityGameMode);
		if (GM)
		{
			GM->OnGameStartedDelegate.AddDynamic(this, &AMR_General::StartGame);
			GM->OnGameEndedDelegate.AddDynamic(this, &AMR_General::EndGame);
		}
	}
}

// REPLICATED PROPS
void AMR_General::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMR_General, BaseInstance);
	DOREPLIFETIME(AMR_General, PlayerDefaultColor);
	DOREPLIFETIME(AMR_General, CurrentlyHoveredModule_L);
	DOREPLIFETIME(AMR_General, CurrentlyHoveredModule_R);
	DOREPLIFETIME(AMR_General, MilitaryBaseComp);
	DOREPLIFETIME(AMR_General, SelectedModuleActor);
	DOREPLIFETIME(AMR_General, bInputIsEnabled);
}
// ~REPLICATED PROPS


// PLAYER INPUT
void AMR_General::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->AddMappingContext(GameplayInputMappingContext, 1);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	// Bindings
	Input->BindAction(IA_SpawnUnit, ETriggerEvent::Started, this, &AMR_General::Action_SpawnUnit);
	Input->BindAction(IA_SelectModule_L, ETriggerEvent::Started, this, &AMR_General::SelectModule_L);
	Input->BindAction(IA_SelectModule_R, ETriggerEvent::Started, this, &AMR_General::SelectModule_R);
}
// ~PLAYER INPUT


// BEGIN PLAY
void AMR_General::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());

	PointerStick_L->SetVisibility(false);
	PointerStick_R->SetVisibility(false);
	
	// SetPlayerColor();

	if (IsLocallyControlled())
	{
		MilitaryBaseComp->SpawnMilitaryBase(this);
		MilitaryBaseComp->SpawnModules(this);
	}
}
// ~BEGIN PLAY

// TICK
void AMR_General::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled() && bInputIsEnabled)
	{
		
		if (!bIsMenuActive)
		{
			if (MotionController_L && ImpactPointer_L)
			{
				PerformSphereTrace(MotionController_L, ImpactPointer_L, CurrentlyHoveredModule_L);
			}
			if (MotionController_R && ImpactPointer_R)
			{
				PerformSphereTrace(MotionController_R, ImpactPointer_R, CurrentlyHoveredModule_R);
			}
		}
	}
}
// ~TICK

void AMR_General::PerformSphereTrace(
	UMotionControllerComponent* UsedController,
	UStaticMeshComponent* ImpactPointer,
	AModuleActor*& CurrentlyHoveredModule)
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

			AModuleActor* HitModule = Cast<AModuleActor>(HitResult.GetActor());
			AMR_General* HittedGeneral = Cast<AMR_General>(HitResult.GetActor()->GetOwner());

			if (HitModule && HittedGeneral == this)
			{
				CurrentlyHoveredModule = HitModule;
				ImpactPointer->SetWorldLocation(CurrentlyHoveredModule->GetActorLocation() + FVector(0.f, 0.f, 8.f));
				PrevisouslyHighlightedModule = HitModule;
				CurrentlyHoveredModule->EnableInfoWidget();
				CurrentlyHoveredModule->SetOverlayMaterial();
				if (CurrentlyHoveredModule != PrevisouslyHighlightedModule)
				{
					PrevisouslyHighlightedModule->DisableInfoWidget();
					PrevisouslyHighlightedModule->RemoveOverlayMaterial();
				}
			}
			else
			{
				if (CurrentlyHoveredModule)
				{
					CurrentlyHoveredModule->DisableInfoWidget();
					CurrentlyHoveredModule = nullptr;
					PrevisouslyHighlightedModule->DisableInfoWidget();
					PrevisouslyHighlightedModule->RemoveOverlayMaterial();
					
				}
			}
		}
		else
		{
			if (CurrentlyHoveredModule)
			{
				CurrentlyHoveredModule->DisableInfoWidget();
				CurrentlyHoveredModule->RemoveOverlayMaterial();
				CurrentlyHoveredModule = nullptr;
				PrevisouslyHighlightedModule->DisableInfoWidget();
				PrevisouslyHighlightedModule->RemoveOverlayMaterial();
				if (PrevisouslyHighlightedModule)
				{
					PrevisouslyHighlightedModule->DisableInfoWidget();
					PrevisouslyHighlightedModule->RemoveOverlayMaterial();
				}
			}

			if (ImpactPointer)
			{
				ImpactPointer->SetWorldLocation(End);
				if (PrevisouslyHighlightedModule)
				{
					PrevisouslyHighlightedModule->DisableInfoWidget();
					PrevisouslyHighlightedModule->RemoveOverlayMaterial();
				}
			}
		}
	}

void AMR_General::EnablePlayerInput()
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

void AMR_General::MovePlayerOnCircle(AActor* Player, float InDelta, float& Angle, float Speed)
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

void AMR_General::StartGame()
{
	EnablePlayerInput();
	PointerStick_L->SetVisibility(true);
	PointerStick_R->SetVisibility(true);
	ResourcesComponent->StartGrowResources();
}

void AMR_General::EndGame(APawn* Looser)
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

void AMR_General::SelectModule_L()
{
	if (CurrentlyHoveredModule_L)
	{
		SelectedModuleActor = CurrentlyHoveredModule_L;
		OnSelectedModuleChanged();
	}
}

void AMR_General::SelectModule_R()
{
	if (CurrentlyHoveredModule_R)
	{
		SelectedModuleActor = CurrentlyHoveredModule_R;
		OnSelectedModuleChanged();
	}
}

void AMR_General::Action_SpawnUnit()
{
	if (IsLocallyControlled())
	{
		MilitaryBaseComp->SpawnUnit(this, SelectedModuleActor);
	}
}

void AMR_General::SpawnPreviewUnit(AModuleActor* ModuleActor)
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
			UStaticMesh* PreviewSM = ModuleActor->BuildingDataAsset->UnitToSpawnData->SM_Unit;
			if (PreviewSM)
			{
				PreviewMeshComponent->SetStaticMesh(PreviewSM);
			}
		}
	}
}

void AMR_General::OnSelectedModuleChanged()
{
	if(PreviewInstance)
	{
		PreviewInstance->Destroy();
	}
	SpawnPreviewUnit(SelectedModuleActor);
}








