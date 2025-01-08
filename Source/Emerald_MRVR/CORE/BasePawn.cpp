#include "BasePawn.h"

#include "CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "PC_MR_General.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	VR_Root = CreateDefaultSubobject<UCapsuleComponent>("VR_Root");
	VR_Root->SetupAttachment(Camera);
	VR_Root->SetIsReplicated(true);
	VR_Root->SetCollisionResponseToAllChannels(ECR_Ignore);
	VR_Root->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	SetRootComponent(VR_Root);

	VR_Proxy = CreateDefaultSubobject<USceneComponent>("VR_Origin");
	VR_Proxy->SetIsReplicated(true);
	VR_Proxy->SetupAttachment(VR_Root);
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetIsReplicated(true);
	Camera->SetupAttachment(VR_Proxy);

	GeneralBody = CreateDefaultSubobject<UStaticMeshComponent>("GeneralBody");
	GeneralBody->SetupAttachment(VR_Proxy);
	GeneralBody->SetIsReplicated(true);
	GeneralBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	MotionController_L = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_L");
	MotionController_L->SetIsReplicated(true);
	MotionController_L->SetupAttachment(VR_Proxy);
	
	MotionController_R = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_R");
	MotionController_R->SetIsReplicated(true);
	MotionController_R->SetupAttachment(VR_Proxy);

	PointerDistance = 2000.f;
	
	ImpactPointer_L = CreateDefaultSubobject<UStaticMeshComponent>("ImpactPointerL");
	ImpactPointer_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ImpactPointer_L->SetIsReplicated(true);
	ImpactPointer_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ImpactPointer_R = CreateDefaultSubobject<UStaticMeshComponent>("ImpactPointerR");
	ImpactPointer_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ImpactPointer_R->SetIsReplicated(true);
	ImpactPointer_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WidgetInteraction_L = CreateDefaultSubobject<UWidgetInteractionComponent>("InteractionLeft");
	WidgetInteraction_L->SetupAttachment(ImpactPointer_L);
	WidgetInteraction_L->SetIsReplicated(true);
	
	WidgetInteraction_R = CreateDefaultSubobject<UWidgetInteractionComponent>("InteractionRight");
	WidgetInteraction_R->SetupAttachment(ImpactPointer_R);
	WidgetInteraction_R->SetIsReplicated(true);

	PointerStick_L = CreateDefaultSubobject<UStaticMeshComponent>("PointerStick_L");
	PointerStick_L->SetupAttachment(MotionController_L);
	PointerStick_L->SetIsReplicated(true);
	PointerStick_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PointerStick_R = CreateDefaultSubobject<UStaticMeshComponent>("PointerStick_R");
	PointerStick_R->SetupAttachment(MotionController_R);
	PointerStick_R->SetIsReplicated(true);
	PointerStick_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePawn, ImpactPointer_L);
	DOREPLIFETIME(ABasePawn, ImpactPointer_R);
	DOREPLIFETIME(ABasePawn, VR_Proxy);
	DOREPLIFETIME(ABasePawn, VR_Root);
	DOREPLIFETIME(ABasePawn, Camera);
	DOREPLIFETIME(ABasePawn, GeneralBody);
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (bIsMenuActive)
	{
		SetUpPointer(MotionController_L, PointerDistance, ImpactPointer_L, WidgetInteraction_L, EControllerHand::Left, HitResultLeft);
		SetUpPointer(MotionController_R, PointerDistance, ImpactPointer_R, WidgetInteraction_R, EControllerHand::Right, HitResultRight);
	}
}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(MenuInputMappingContext, 0);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(Click, ETriggerEvent::Started, this, &ABasePawn::OnMousePressed);
	Input->BindAction(Click, ETriggerEvent::Triggered, this, &ABasePawn::OnMousePressed);
	Input->BindAction(Click, ETriggerEvent::Completed, this, &ABasePawn::OnMouseReleased);
}

void ABasePawn::OnMousePressed()
{
	WidgetInteraction_L->PressPointerKey(EKeys::LeftMouseButton);
	WidgetInteraction_R->PressPointerKey(EKeys::LeftMouseButton);
}

void ABasePawn::OnMouseReleased()
{
	WidgetInteraction_L->ReleasePointerKey(EKeys::LeftMouseButton);
	WidgetInteraction_R->ReleasePointerKey(EKeys::LeftMouseButton);
}


// Setup Pointer
void ABasePawn::SetUpPointer(UMotionControllerComponent* MotionControllerComponent, float Pointerdistance,
	UStaticMeshComponent* ImpactPointer, UWidgetInteractionComponent* WidgetInteractionComponent, EControllerHand Hand, FHitResult& HitResult)
{
	UHeadMountedDisplayFunctionLibrary* HMDLibrary;
	FXRMotionControllerData FxrMotionControllerData;
	HMDLibrary->GetMotionControllerData(MotionControllerComponent, Hand, FxrMotionControllerData);

	FVector Start = FxrMotionControllerData.AimPosition;
	FVector End = FxrMotionControllerData.AimRotation.GetForwardVector() * PointerDistance;

	UWorld* World = GetWorld();
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
		WidgetInteractionComponent->SetRelativeRotation(WidgetinteractionRotation * -1);
	}
}
//~Setup Pointer