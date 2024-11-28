#include "MR_General.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EK_GameMode.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Components/StaticMeshComponent.h"
#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"
#include "Net/UnrealNetwork.h"
#include "HeadMountedDisplayFunctionLibrary.h"
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
	// ~COMPONENTS

}

// REPLICATED PROPS
void AMR_General::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMR_General, ReplicatedPosition);
	DOREPLIFETIME(AMR_General, ReplicatedRotation);
	DOREPLIFETIME(AMR_General, BaseInstance);
	DOREPLIFETIME(AMR_General, AvailableBuildings);
	DOREPLIFETIME(AMR_General, CurrentlySelectedModule);
	DOREPLIFETIME(AMR_General, PlayerDefaultColor);

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
}
// ~PLAYER INPUT


// BEGIN PLAY
void AMR_General::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	SetPlayerColor();
	
	// Jsem Server	
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

	if (MotionController_L && MotionController_R && BaseInstance)
	{
		bGameInitialized = true;
	}

	if (IsLocallyControlled())
	{
		FVector HMDPosition;
		FRotator HMDOrientation;
		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDOrientation, HMDPosition);
		Server_UpdatePawnPosition(HMDPosition, HMDOrientation);
		if (bGameInitialized)
		{
			SetUpPointer(MotionController_L, PointerDistance, ImpactPointer_L, WidgetInteraction_L, EControllerHand::Left, HitResultLeft);
			SetUpPointer(MotionController_R, PointerDistance, ImpactPointer_R, WidgetInteraction_R, EControllerHand::Right, HitResultRight);
		}
		
	}
}
// ~TICK

// UPDATE PAWN MOVEMENT
void AMR_General::Server_UpdatePawnPosition_Implementation(const FVector& NewPosition, const FRotator& NewRotation)
{
	ReplicatedPosition = NewPosition;
	ReplicatedRotation = NewRotation;
}
// ~UPDATE PAWN MOVEMENT

void AMR_General::SetPlayerColor() // Set Player Color
{
	if (GameMode && GameMode->PlayersColors.Num() > 0)
	{
		PlayerDefaultColor = GameMode->PlayersColors.Pop();
	}
	GeneralBody->SetMaterial(0, PlayerDefaultColor);
	PointerStick_L->SetMaterial(0, PlayerDefaultColor);
	PointerStick_R->SetMaterial(0, PlayerDefaultColor);
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







