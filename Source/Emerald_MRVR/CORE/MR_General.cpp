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
#include "Emerald_MRVR/Unit.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

AMR_General::AMR_General()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	// bAlwaysRelevant = true;

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
	DOREPLIFETIME(AMR_General, ReplicatedPosition);
	DOREPLIFETIME(AMR_General, ReplicatedRotation);
	DOREPLIFETIME(AMR_General, BaseInstance);
	DOREPLIFETIME(AMR_General, PC);
}

void AMR_General::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Bindings
	Input->BindAction(DebugSpawnUnit, ETriggerEvent::Started, this, &AMR_General::SpawnUnit);
	
}
void AMR_General::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMR_General::BeginPlay()
{
	Super::BeginPlay();
	PC = Cast<APC_MR_General>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController()) ; 
	GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	
	if (GameMode && HasAuthority())
	{
		Server_SpawnMilitaryBase(MilitaryBase);
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

void AMR_General::Server_UpdatePawnPosition_Implementation(const FVector& NewPosition, const FRotator& NewRotation)
{
	ReplicatedPosition = NewPosition;
	ReplicatedRotation = NewRotation;
}

void AMR_General::Server_SpawnMilitaryBase_Implementation(TSubclassOf<AMilitaryBase> Base)
{
	TArray<ATargetPoint*> TargetPoints = GameMode->GetAllTargetpoints();
	if (GameMode && (GameMode->TargetPoints.Num() > 0))
	{
		TargetPoint = GameMode->TargetPoints.IsValidIndex(0) ? GameMode->TargetPoints[0] : nullptr;
		SpawnPoint = GameMode->TargetPoints[0]->GetTransform();
		GameMode->TargetPoints.RemoveAt(0);

		if (TargetPoint)
		{
			FVector SpawnLocation = TargetPoint->GetActorLocation();
			FRotator SpawnRotation = TargetPoint->GetActorRotation();
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = this;
			//SpawnParameters.Owner = GetWorld()->GetGameInstance()->GetFirstLocalPlayerController();
			BaseInstance = GetWorld()->SpawnActor<AMilitaryBase>(Base, SpawnLocation, SpawnRotation, SpawnParameters);
		}
	}
}

void AMR_General::SpawnUnit()
{
	if (HasAuthority())
	{
		Multi_SpawnUnit(UnitToSpawnClass);
	}
	else
	{
		Server_SpawnUnit(UnitToSpawnClass);
	}
}

void AMR_General::Server_SpawnUnit_Implementation(TSubclassOf<AUnit> UnitToSpawn)
{
	Multi_SpawnUnit(UnitToSpawn);
}

void AMR_General::Multi_SpawnUnit_Implementation(TSubclassOf<AUnit> UnitToSpawn)
{
	FVector Location = BaseInstance->SpawnPoint_Ground->GetComponentLocation();
	FRotator Rotation = BaseInstance->SpawnPoint_Ground->GetComponentRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	if (UnitToSpawn)
	{
		GetWorld()->SpawnActor<AUnit>(UnitToSpawn, Location, Rotation, SpawnParams);
	}
}
