#include "PrimitiveVRPawn.h"

#include "CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

APrimitiveVRPawn::APrimitiveVRPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	VR_Root = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, "VR_Root");
	VR_Root->SetupAttachment(Camera);
	VR_Root->SetIsReplicated(true);
	VR_Root->SetCollisionResponseToAllChannels(ECR_Ignore);
	VR_Root->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	SetRootComponent(VR_Root);

	VR_Proxy = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"VR_Origin");
	VR_Proxy->SetIsReplicated(true);
	VR_Proxy->SetupAttachment(VR_Root);
	
	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this,"Camera");
	Camera->SetIsReplicated(true);
	Camera->SetupAttachment(VR_Proxy);

	Body = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,"PawnBody");
	Body->SetupAttachment(VR_Proxy);
	Body->SetIsReplicated(true);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	MotionController_L = ObjectInitializer.CreateDefaultSubobject<UMotionControllerComponent>(this,"Motion_Controller_L");
	MotionController_L->SetIsReplicated(true);
	MotionController_L->SetupAttachment(VR_Proxy);
	
	MotionController_R = ObjectInitializer.CreateDefaultSubobject<UMotionControllerComponent>(this,"Motion_Controller_R");
	MotionController_R->SetIsReplicated(true);
	MotionController_R->SetupAttachment(VR_Proxy);

}

void APrimitiveVRPawn::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	EKGameState = World->GetGameState<AEKGameState>();
	if (!EKGameState)
	{
		UE_LOG(LogTemp, Error, TEXT("Primitive GameMode: GameState is NULL"));
	}
	
}

void APrimitiveVRPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APrimitiveVRPawn, VR_Proxy);
	DOREPLIFETIME(APrimitiveVRPawn, VR_Root);
	DOREPLIFETIME(APrimitiveVRPawn, Camera);
	DOREPLIFETIME(APrimitiveVRPawn, Body);
}

void APrimitiveVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrimitiveVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

