#include "MR_General.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EK_GameMode.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"
#include "Net/UnrealNetwork.h"
#include "MotionControllerComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/ModuleActor.h"
#include "Emerald_MRVR/Components/MilitaryBaseComp.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
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
	
	DOREPLIFETIME(AMR_General, BaseInstance);
	DOREPLIFETIME(AMR_General, PlayerDefaultColor);
	DOREPLIFETIME(AMR_General, CurrentlyHoveredModule_L);
	DOREPLIFETIME(AMR_General, CurrentlyHoveredModule_R);
	DOREPLIFETIME(AMR_General, MilitaryBaseComp);
	DOREPLIFETIME(AMR_General, SelectedModuleActor);

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
	Input->BindAction(IA_SpawnUnit, ETriggerEvent::Started, this, &AMR_General::Action_SpawnUnit);
	Input->BindAction(IA_SelectModule_L, ETriggerEvent::Started, this, &AMR_General::SelectModule_L);
	Input->BindAction(IA_SelectModule_R, ETriggerEvent::Started, this, &AMR_General::SelectModule_R);
}
// ~PLAYER INPUT


// BEGIN PLAY
void AMR_General::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	SetPlayerColor();

	if (IsLocallyControlled())
	{
		MilitaryBaseComp->SpawnMilitaryBase(this);
		MilitaryBaseComp->SpawnModules(this);
	}
}
// ~BEGIN PLAY

// TICK
void AMR_General::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		
		if (!bIsMenuActive)
		{
			if (MotionController_L && ImpactPointer_L)
			{
				PerformSphereTrace(MotionController_L, ImpactPointer_L, CurrentlyHoveredModule_L);
			}
			if (MotionController_R && ImpactPointer_R)
			{
				PerformSphereTrace(MotionController_R, ImpactPointer_R, CurrentlyHoveredModule_R);
			}
			
		}
	}
}
// ~TICK

void AMR_General::SetPlayerColor() // Sets Player Color
{
	if (GameMode && GameMode->PlayersColors.Num() > 0)
	{
		PlayerDefaultColor = GameMode->PlayersColors.Pop();
	}
	GeneralBody->SetMaterial(0, PlayerDefaultColor);
	PointerStick_L->SetMaterial(0, PlayerDefaultColor);
	PointerStick_R->SetMaterial(0, PlayerDefaultColor);
}

void AMR_General::PerformSphereTrace(
	TObjectPtr<UMotionControllerComponent> UsedController,
	TObjectPtr<UStaticMeshComponent> ImpactPointer,
	AModuleActor*& CurrentlyHoveredModule)
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FVector Start = UsedController->GetComponentLocation();
	FVector End = Start + (UsedController->GetForwardVector() * 3000.f);
	float Radius = 5.f;

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(Radius),
		QueryParams);

	AModuleActor* PrevisouslyHighlightedModule = nullptr;
	if (bHit)
	{
		if (ImpactPointer)
		{
			ImpactPointer->SetWorldLocation(HitResult.Location);
		}

		AModuleActor* HitModule = Cast<AModuleActor>(HitResult.GetActor());
		AMR_General* HittedGeneral = Cast<AMR_General>(HitResult.GetActor()->GetOwner());

		if (HitModule && HittedGeneral == this)
		{
			CurrentlyHoveredModule = HitModule;
			HitModule->HighlightModule(true);
		}
		else
		{
			if (CurrentlyHoveredModule)
			{
				CurrentlyHoveredModule->HighlightModule(false);
				CurrentlyHoveredModule = nullptr;
			}
		}
	}
	else
	{
		if (CurrentlyHoveredModule)
		{
			CurrentlyHoveredModule->HighlightModule(false);
			CurrentlyHoveredModule = nullptr;
		}

		if (ImpactPointer)
		{
			ImpactPointer->SetWorldLocation(End);
		}
	}
}

void AMR_General::SelectModule_L()
{
	if (CurrentlyHoveredModule_L)
	{
		SelectedModuleActor = CurrentlyHoveredModule_L;
		OnSelectedModuleChanged();
	}
}

void AMR_General::SelectModule_R()
{
	if (CurrentlyHoveredModule_R)
	{
		SelectedModuleActor = CurrentlyHoveredModule_R;
		OnSelectedModuleChanged();
	}
}

void AMR_General::Action_SpawnUnit()
{
	if (IsLocallyControlled())
	{
		MilitaryBaseComp->SpawnUnit(this, SelectedModuleActor);
	}
}


void AMR_General::SpawnPreviewUnit(AModuleActor* ModuleActor)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SpawnLoc = MotionController_R->GetComponentLocation() + FVector(0.f,0.f,15.f);
		FRotator SpawnRot = FRotator::ZeroRotator;
		PreviewInstance = World->SpawnActor<AActor>(PreviewUnitClass, SpawnLoc, SpawnRot, SpawnParameters);
		UStaticMeshComponent* PreviewMeshComponent = PreviewInstance->FindComponentByClass<UStaticMeshComponent>();
		PreviewMeshComponent->AttachToComponent(MotionController_R, FAttachmentTransformRules::KeepRelativeTransform);
		if (PreviewMeshComponent)
		{
			UStaticMesh* PreviewSM = ModuleActor->BuildingDataAsset->UnitToSpawnData->SM_Unit;
			if (PreviewSM)
			{
				PreviewMeshComponent->SetStaticMesh(PreviewSM);
			}
		}
	}
}

void AMR_General::OnSelectedModuleChanged()
{
	if(PreviewInstance)
	{
		PreviewInstance->Destroy();
	}
	SpawnPreviewUnit(SelectedModuleActor);
}







