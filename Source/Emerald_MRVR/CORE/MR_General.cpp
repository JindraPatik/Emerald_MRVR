#include "MR_General.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EK_GameMode.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/GameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"
#include "Net/UnrealNetwork.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "PC_MR_General.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBaseComp.h"
#include "Emerald_MRVR/Unit.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

AMR_General::AMR_General()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	RootComponent = Camera;

	GeneralBody = CreateDefaultSubobject<UStaticMeshComponent>("GeneralBody");
	GeneralBody->SetupAttachment(RootComponent);
	Hands = CreateDefaultSubobject<USceneComponent>("Hands");
	
	MotionController_L = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_L");
	MotionController_L->SetupAttachment(Hands);
	
	MotionController_R = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_R");
	MotionController_R->SetupAttachment(Hands);

	ImpactPointer_L = CreateDefaultSubobject<UStaticMeshComponent>("ImpactPointerL");
	ImpactPointer_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ImpactPointer_R = CreateDefaultSubobject<UStaticMeshComponent>("ImpactPointerR");
	ImpactPointer_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// COMPONENTS
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>("Resources");
	MilitaryBaseComp = CreateDefaultSubobject<UMilitaryBaseComp>("MilitaryBaseComp");
	// ~COMPONENTS

}

// REPLICATED PROPS
void AMR_General::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMR_General, ReplicatedPosition);
	DOREPLIFETIME(AMR_General, ReplicatedRotation);
	DOREPLIFETIME(AMR_General, BaseInstance);
	DOREPLIFETIME(AMR_General, PC);
}
// ~REPLICATED PROPS


// PLAYER INPUT
void AMR_General::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bindings
	Input->BindAction(DebugSpawnUnit, ETriggerEvent::Started, this, &AMR_General::Action_SpawnUnit);
	
}
// ~PLAYER INPUT


// BEGIN PLAY
void AMR_General::BeginPlay()
{
	Super::BeginPlay();
	PC = Cast<APC_MR_General>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController()) ; 
	GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	
	if (IsLocallyControlled())
	{
		MilitaryBaseComp->Server_SpawnMilitaryBase(this);
	}
}

void AMR_General::SpawnMilitaryBase()
{
	if (IsLocallyControlled())
	{
		MilitaryBaseComp->Server_SpawnMilitaryBase(this);
	}
}

// ~BEGIN PLAY


void AMR_General::Action_SpawnUnit()
{
	MilitaryBaseComp->SpawnUnit();
}

// TICK
void AMR_General::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		FVector HMDPosition;
		FRotator HMDOrientation;
		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDOrientation, HMDPosition);
		Server_UpdatePawnPosition(HMDPosition, HMDOrientation);
	}
}

void AMR_General::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

// ~TICK

/*void AMR_General::SetUnitTargetLoc()
{
	// UnitTargetLoc = BaseInstance->SpawnPoint_Ground->GetComponentLocation();
}*/

// UPDATE PAWN MOVEMENT
void AMR_General::Server_UpdatePawnPosition_Implementation(const FVector& NewPosition, const FRotator& NewRotation)
{
	ReplicatedPosition = NewPosition;
	ReplicatedRotation = NewRotation;
}
// ~UPDATE PAWN MOVEMENT


