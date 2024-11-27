#include "MR_General.h"

#include "DelayAction.h"
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
#include "Components/WidgetInteractionComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/Components/MilitaryBaseComp.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/Components/BuildingsModuleComponent.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Emerald_MRVR/EKG_Enums.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AMR_General::AMR_General()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetIsReplicated(true);

	RootComponent = Camera;
	RootComponent->SetIsReplicated(true);

	GeneralBody = CreateDefaultSubobject<UStaticMeshComponent>("GeneralBody");
	GeneralBody->SetupAttachment(RootComponent);
	GeneralBody->SetIsReplicated(true);
	Hands = CreateDefaultSubobject<USceneComponent>("Hands");
	
	MotionController_L = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_L");
	MotionController_L->SetIsReplicated(true);
	MotionController_L->SetupAttachment(Hands);
	
	MotionController_R = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_R");
	MotionController_R->SetIsReplicated(true);
	MotionController_R->SetupAttachment(Hands);

	PointerDistance = 2000.f;
	ImpactPointer_L = CreateDefaultSubobject<UStaticMeshComponent>("ImpactPointerL");
	ImpactPointer_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ImpactPointer_R = CreateDefaultSubobject<UStaticMeshComponent>("ImpactPointerR");
	ImpactPointer_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WidgetInteraction_L = CreateDefaultSubobject<UWidgetInteractionComponent>("InteractionLeft");
	WidgetInteraction_L->SetupAttachment(ImpactPointer_L);
	
	WidgetInteraction_R = CreateDefaultSubobject<UWidgetInteractionComponent>("InteractionRight");
	WidgetInteraction_R->SetupAttachment(ImpactPointer_R);


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
;
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

// Post initialize comp
void AMR_General::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
// ~Post initialize comp

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
}

// Setup Pointer
void AMR_General::SetUpPointer(UMotionControllerComponent* MotionControllerComponent, float Pointerdistance,
	UStaticMeshComponent* ImpactPointer, UWidgetInteractionComponent* WidgetInteractionComponent, EControllerHand Hand, FHitResult& HitResult)
{
	UHeadMountedDisplayFunctionLibrary* HMDLibrary;
	FXRMotionControllerData FxrMotionControllerData;
	HMDLibrary->GetMotionControllerData(MotionControllerComponent, Hand, FxrMotionControllerData);

	FVector Start = FxrMotionControllerData.AimPosition;
	FVector End = FxrMotionControllerData.AimRotation.GetForwardVector() * PointerDistance;

	TObjectPtr<UWorld> World = GetWorld();
	if (World)
	{
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;
		QueryParams.AddIgnoredActor(this);
        
		bool	bHit = World->SweepSingleByChannel(
				HitResult,
				Start,
				End,
				FQuat::Identity,
				ECC_Visibility,
				FCollisionShape::MakeSphere(5.f),
				QueryParams);
        
		ImpactPointer->SetWorldLocation(HitResult.ImpactPoint);
		FRotator WidgetinteractionRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), HitResult.ImpactPoint);
		WidgetInteractionComponent->SetRelativeRotation(WidgetinteractionRotation);
        
		if (bHit)
		{
			DetectModule(HitResult);
		}
	}
}
//~Setup Pointer


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

void AMR_General::SelectBuilding()
{
	for (UBuildingDataAsset* Building : AvailableBuildings)
	{
		
	}
}

void AMR_General::DetectModule(FHitResult HitResult)
{
	if (GEngine)
	{
		//FString Message = FString::Printf(TEXT("Hitted Component: %s"), *HitResult.Component->GetName());
		FString Message = FString::Printf(TEXT("Hitted Component: %s"), *HitResult.Component->GetReadableName());
		GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Orange, Message);
	}
	
}




