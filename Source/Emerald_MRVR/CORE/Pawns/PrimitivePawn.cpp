#include "PrimitivePawn.h"

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

APrimitivePawn::APrimitivePawn()
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

	Body = CreateDefaultSubobject<UStaticMeshComponent>("PawnBody");
	Body->SetupAttachment(VR_Proxy);
	Body->SetIsReplicated(true);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	MotionController_L = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_L");
	MotionController_L->SetIsReplicated(true);
	MotionController_L->SetupAttachment(VR_Proxy);
	
	MotionController_R = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_R");
	MotionController_R->SetIsReplicated(true);
	MotionController_R->SetupAttachment(VR_Proxy);

}

void APrimitivePawn::BeginPlay()
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

void APrimitivePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APrimitivePawn, VR_Proxy);
	DOREPLIFETIME(APrimitivePawn, VR_Root);
	DOREPLIFETIME(APrimitivePawn, Camera);
	DOREPLIFETIME(APrimitivePawn, Body);
}

void APrimitivePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrimitivePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

