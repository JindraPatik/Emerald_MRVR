#include "BasePawn.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "MR_General.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "GameFramework/GameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

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
	ImpactPointer_L->SetIsReplicated(true);

	ImpactPointer_R = CreateDefaultSubobject<UStaticMeshComponent>("ImpactPointerR");
	ImpactPointer_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ImpactPointer_R->SetIsReplicated(true);

	WidgetInteraction_L = CreateDefaultSubobject<UWidgetInteractionComponent>("InteractionLeft");
	WidgetInteraction_L->SetupAttachment(ImpactPointer_L);
	WidgetInteraction_L->SetIsReplicated(true);
	
	WidgetInteraction_R = CreateDefaultSubobject<UWidgetInteractionComponent>("InteractionRight");
	WidgetInteraction_R->SetupAttachment(ImpactPointer_R);
	WidgetInteraction_R->SetIsReplicated(true);

	PointerStick_L = CreateDefaultSubobject<UStaticMeshComponent>("PointerStick_L");
	PointerStick_L->SetupAttachment(MotionController_L);
	PointerStick_L->SetIsReplicated(true);

	PointerStick_R = CreateDefaultSubobject<UStaticMeshComponent>("PointerStick_R");
	PointerStick_R->SetupAttachment(MotionController_R);
	PointerStick_R->SetIsReplicated(true);

}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePawn, Camera);
	DOREPLIFETIME(ABasePawn, GeneralBody);
	DOREPLIFETIME(ABasePawn, MotionController_L);
	DOREPLIFETIME(ABasePawn, MotionController_R);
	DOREPLIFETIME(ABasePawn, ImpactPointer_L);
	DOREPLIFETIME(ABasePawn, ImpactPointer_R);
	DOREPLIFETIME(ABasePawn, PointerStick_L);
	DOREPLIFETIME(ABasePawn, PointerStick_R);
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (GetWorld()->GetGameState()->IsActorInitialized())
	{
		SetUpPointer(MotionController_L, PointerDistance, ImpactPointer_L, WidgetInteraction_L, EControllerHand::Left, HitResultLeft);
		SetUpPointer(MotionController_R, PointerDistance, ImpactPointer_R, WidgetInteraction_R, EControllerHand::Right, HitResultRight);
	}
}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
	}
}
//~Setup Pointer