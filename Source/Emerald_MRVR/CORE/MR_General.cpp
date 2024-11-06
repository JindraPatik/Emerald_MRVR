#include "MR_General.h"

#include "EK_GameMode.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/GameMode.h"


AMR_General::AMR_General()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	RootComponent = Camera;
	
	SM_Body = CreateDefaultSubobject<UStaticMesh>("Body");

	Hands = CreateDefaultSubobject<USceneComponent>("Hands");
	
	MotionController_L = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_L");
	MotionController_L->SetupAttachment(Hands);
	
	MotionController_R = CreateDefaultSubobject<UMotionControllerComponent>("Motion_Controller_R");
	MotionController_R->SetupAttachment(Hands);
}

void AMR_General::BeginPlay()
{
	Super::BeginPlay();

	AEK_GameMode* GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// Picks first element in array and destroy it
		ATargetPoint* TargetPoint = GameMode->TargetPoints.IsValidIndex(0) ? GameMode->TargetPoints[0] : nullptr;
		GameMode->TargetPoints.RemoveAt(0);
		if (TargetPoint)
		{
			FVector SpawnLocation = TargetPoint->GetActorLocation();
			FRotator SpawnRotation = TargetPoint->GetActorRotation();
			SpawnMilitarybase(MilitaryBase, SpawnLocation, SpawnRotation);	
		}
		return GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("Target points are empty")));
	}
}

void AMR_General::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMR_General::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

AActor* AMR_General::SpawnMilitarybase(TSubclassOf<AMilitaryBase> Base, FVector Location, FRotator Rotation)
{
	if (!Base) return nullptr;
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	
	BaseInstance = GetWorld()->SpawnActor<AMilitaryBase>(Base, Location, Rotation, SpawnParameters);
	return BaseInstance;
}

