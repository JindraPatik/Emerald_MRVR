#include "MR_General.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EK_GameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"
#include "Net/UnrealNetwork.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/Components/BuildingsModuleComponent.h"
#include "Emerald_MRVR/Components/MilitaryBaseComp.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "GameFramework/GameStateBase.h"

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

	CurrentlySelectedModule = nullptr;
	// ~COMPONENTS

}

// REPLICATED PROPS
void AMR_General::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMR_General, BaseInstance);
	DOREPLIFETIME(AMR_General, AvailableBuildings);
	DOREPLIFETIME(AMR_General, CurrentlySelectedModule);
	DOREPLIFETIME(AMR_General, PlayerDefaultColor);
	DOREPLIFETIME(AMR_General, CurrentlyHoveredModule_L);
	DOREPLIFETIME(AMR_General, CurrentlyHoveredModule_R);

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
	Input->BindAction(DebugSpawnUnit, ETriggerEvent::Started, this, &AMR_General::Action_SpawnUnit);
	Input->BindAction(Action_SelectModule_L, ETriggerEvent::Started, this, &AMR_General::SelectModule_L);
	Input->BindAction(Action_SelectModule_R, ETriggerEvent::Started, this, &AMR_General::SelectModule_R);
}
// ~PLAYER INPUT


// BEGIN PLAY
void AMR_General::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	SetPlayerColor();
	
	if (IsLocallyControlled())
	{
		MilitaryBaseComp->Server_SpawnMilitaryBase(this);
	}
}
// ~BEGIN PLAY

// TICK
void AMR_General::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		if (bPossesed)
		{
			FVector HMDPosition;
			FRotator HMDOrientation;
			UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDOrientation, HMDPosition);
			if (HasAuthority())
			{
				UpadatePosition(HMDPosition, HMDOrientation);
			}
			else
			{
				Server_UpdatePawnPosition(HMDPosition, HMDOrientation);
			}
			
		}
		
		// if (bGameInitialized)
		{
			//SetUpPointer(MotionController_L, PointerDistance, ImpactPointer_L, WidgetInteraction_L, EControllerHand::Left, HitResultLeft);
			//SetUpPointer(MotionController_R, PointerDistance, ImpactPointer_R, WidgetInteraction_R, EControllerHand::Right, HitResultRight);
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

void AMR_General::SetPlayerColor() // Sets Player Color
{
	if (GameMode && GameMode->PlayersColors.Num() > 0)
	{
		PlayerDefaultColor = GameMode->PlayersColors.Pop();
	}
	GeneralBody->SetMaterial(0, PlayerDefaultColor);
	PointerStick_L->SetMaterial(0, PlayerDefaultColor);
	PointerStick_R->SetMaterial(0, PlayerDefaultColor);
}

void AMR_General::PerformSphereTrace(TObjectPtr<UMotionControllerComponent> UsedController,
	TObjectPtr<UStaticMeshComponent> ImpactPointer,
	UBuildingsModuleComponent*& CurrentlyHoveredModule)
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

		//UBuildingsModuleComponent* HitModule = Cast<UBuildingsModuleComponent>(HitResult.GetActor()->GetComponentByClass(UBuildingsModuleComponent::StaticClass()));
		UBuildingsModuleComponent* HitModule = Cast<UBuildingsModuleComponent>(HitResult.GetComponent()->GetAttachParent());

		if (HitModule)
		{
			CurrentlyHoveredModule = HitModule;
			HitModule->HighlightModule(true);
		}
		else
		{
			if (CurrentlyHoveredModule)
			{
				CurrentlyHoveredModule->HighlightModule(false);
				CurrentlyHoveredModule = nullptr;
			}
		}
	}
	else
	{
		if (CurrentlyHoveredModule)
		{
			CurrentlyHoveredModule->HighlightModule(false);
			CurrentlyHoveredModule = nullptr;
		}

		if (ImpactPointer)
		{
			ImpactPointer->SetWorldLocation(End);
		}
	}
	
}

void AMR_General::SelectModule_L()
{
	if (CurrentlyHoveredModule_L)
	{
		DBG(5, "Module L")
		CurrentlySelectedModule = CurrentlyHoveredModule_L;
		MilitaryBaseComp->UnitToSpawn = CurrentlySelectedModule->BuildingDataAsset->UnitToSpawn;
		OnSelectedModule();
	}
}

void AMR_General::SelectModule_R()
{
	if (CurrentlyHoveredModule_R)
	{
		DBG(5, "Module R")
		CurrentlySelectedModule = CurrentlyHoveredModule_R;

		// toto vyhazuje chybu !!
		if (MilitaryBaseComp && CurrentlySelectedModule)
		{
			MilitaryBaseComp->UnitToSpawn = CurrentlySelectedModule->BuildingDataAsset->UnitToSpawn;
		}
		// OnSelectedModule();
	}
}

void AMR_General::OnSelectedModule()
{
	if (CurrentlySelectedModule)
	{
		DBG(3, "Module Selected")
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Red, FString::Printf(TEXT("dasda %s"),*CurrentlySelectedModule.GetName()));
		CurrentlySelectedModule->ModuleMesh->SetVisibility(false);
	}
}

void AMR_General::Server_UpdatePawnPosition_Implementation(FVector HMDPosition, FRotator HMDOrientation)
{
	Server_UpdatePawnPosition(HMDPosition, HMDOrientation);
}

void AMR_General::UpadatePosition(FVector HMDPosition, FRotator HMDOrientation)
{
	SetActorLocation(HMDPosition);
	SetActorRotation(FRotator(0.f, HMDOrientation.Yaw, 0.f));
}

// Volá PC
void AMR_General::SpawnMilitaryBase()
{
	if (IsLocallyControlled())
	{
		MilitaryBaseComp->Server_SpawnMilitaryBase(this);
	}
}

void AMR_General::Action_SpawnUnit()
{
	MilitaryBaseComp->SpawnUnit();
}

