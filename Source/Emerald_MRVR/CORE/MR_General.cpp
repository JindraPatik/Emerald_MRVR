#include "MR_General.h"

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


AMR_General::AMR_General()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

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

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>("Resources");

}

void AMR_General::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// ?? replikuje se ??
	DOREPLIFETIME(AMR_General, GeneralBody);
}

void AMR_General::BeginPlay()
{
	Super::BeginPlay();
	// kvuli lobby
	if (AEK_GameMode* GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		Server_SpawnMilitaryBase(MilitaryBase);

		SetInitialPawnPosition(GameMode);
	}
}

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

void AMR_General::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMR_General::SetInitialPawnPosition(AEK_GameMode* GameMode)
{
	if (IsLocallyControlled())
	{
		FVector HMDPosition = GameMode->SelectedPlayerStart->GetActorLocation();
		FRotator HMDOrientation = GameMode->SelectedPlayerStart->GetActorRotation();
			
		Server_UpdatePawnPosition(HMDPosition, HMDOrientation);
	}
}

void AMR_General::Server_UpdatePawnPosition_Implementation(const FVector& NewPosition, const FRotator& NewRotation)
{
	ReplicatedPosition = NewPosition;
	ReplicatedRotation = NewRotation;
}

void AMR_General::Server_SpawnMilitaryBase_Implementation(TSubclassOf<AMilitaryBase> Base)
{
	AEK_GameMode* GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	TArray<ATargetPoint*> TargetPoints = GameMode->GetAllTargetpoints();
	if (GameMode && (GameMode->TargetPoints.Num() > 0))
	{
		TargetPoint = GameMode->TargetPoints.IsValidIndex(0) ? GameMode->TargetPoints[0] : nullptr;
		GameMode->TargetPoints.RemoveAt(0);

		//
		if (TargetPoint)
		{
			FVector SpawnLocation = TargetPoint->GetActorLocation();
			FRotator SpawnRotation = TargetPoint->GetActorRotation();
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = GetInstigator();
			BaseInstance = GetWorld()->SpawnActor<AMilitaryBase>(Base, SpawnLocation, SpawnRotation, SpawnParameters);
		}
		
	}

}

