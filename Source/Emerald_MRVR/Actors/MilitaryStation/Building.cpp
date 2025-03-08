#include "Building.h"
#include "Blueprint/WidgetTree.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"
#include "Components/ProgressBar.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Emerald_MRVR/Widgets/CooldownActor.h"
#include "Interaction/IsdkInteractableComponent.h"
#include "Interaction/IsdkRayInteractable.h"
#include "Interaction/Surfaces/IsdkPointableBox.h"

ABuilding::ABuilding(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	DownScaleComponent = ObjectInitializer.CreateDefaultSubobject<UDownScaleComponent>(this,"DownscaleComponent");

	SceneRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"SceneRoot");
	SetRootComponent(SceneRoot);
	
	BuildingMeshRoot = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,"ModuleBody");
	BuildingMeshRoot->SetupAttachment(SceneRoot);

	InfoWidgetSpawnPoint = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"InfowidgetSpawnPoint");
	InfoWidgetSpawnPoint->SetupAttachment(RootComponent);

	UnitReturnPoint = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"UnitReturnPoint");
	UnitReturnPoint->SetupAttachment(SceneRoot);

	IsdkInteractable = ObjectInitializer.CreateDefaultSubobject<UIsdkRayInteractable>(this,"InteractableComponent");
	IsdkInteractable->SetupAttachment(SceneRoot);

	IsdkPointableBox = ObjectInitializer.CreateDefaultSubobject<UIsdkPointableBox>(this,"PointableBox");
	IsdkPointableBox->SetupAttachment(SceneRoot);
}

void ABuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuilding, BuildingMeshRoot);
	DOREPLIFETIME(ABuilding, BuildingDataAsset);
	DOREPLIFETIME(ABuilding, SceneRoot);
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();


	VRPawn = Cast<AVRPawn>(GetOwner());
	if (!VRPawn)
	{
	}
	
	IsdkPointableBox->SetSize(GetActorBoundingBoxExtent() * FVector(IsdkPointableBoxMultiplier, IsdkPointableBoxMultiplier, IsdkPointableBoxMultiplier));
	IsdkInteractable->SetSurface(IsdkPointableBox);

	if (!IsdkInteractable)
	{
		return;
	}
	IsdkInteractable->GetInteractionPointerEventDelegate().AddUniqueDynamic(this, &ABuilding::HandlePointerEvent);
}

void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Cooldown progress
	if (CooldownWidgetInstance && CooldownDuration > 0.0f)
	{
		CooldownWidgetInstance->SetActorHiddenInGame(false);
		ElapsedTime += DeltaTime;

		float Progress = FMath::Clamp(ElapsedTime / CooldownDuration, 0.0f, 1.0f);

		UWidgetComponent* WidgetComp = CooldownWidgetInstance->FindComponentByClass<UWidgetComponent>();
		if (!WidgetComp)
		{
			return;
		}
		
		UUserWidget* UserWidget = Cast<UUserWidget>(WidgetComp->GetWidget());
		if (!UserWidget)
		{
			return;
		}
		
		UProgressBar* CooldownProgress = Cast<UProgressBar>(UserWidget->WidgetTree->FindWidget("Progress_CoolDown"));
		if (!CooldownProgress)
		{
			return;
		}
		CooldownProgress->SetPercent(Progress);
		
		if (ElapsedTime >= CooldownDuration)
		{
			CooldownWidgetInstance->SetActorHiddenInGame(true);
			SetActorTickEnabled(false);
		}
	}
}

void ABuilding::HandlePointerEvent(const FIsdkInteractionPointerEvent& PointerEvent)
{
	if (!VRPawn)
	{
		return;
	}
	
	switch (PointerEvent.Type)
	{
	case EIsdkPointerEventType::Hover:
		EnableInfoWidget();
		SetOverlayMaterial();
		break;
		
	case EIsdkPointerEventType::Unhover:
		RemoveOverlayMaterial();
		DisableInfoWidget();
		break;
		
	case EIsdkPointerEventType::Select:
		VRPawn->SelectedBuildingActor = this;
		VRPawn->OnSelectedModuleChanged();
		break;
		
	case EIsdkPointerEventType::Unselect:
		;
		break;
		
	case EIsdkPointerEventType::Move:
		;
		break;
		
	case EIsdkPointerEventType::Cancel:
		;
		break;
	}
}

void ABuilding::EnableSpawning()
{
	bSpawningEnabled = true;
}

void ABuilding::Cooldown(float CD_Time)
{
	CooldownDuration = CD_Time;
	ElapsedTime = 0.f;
	GetWorld()->GetTimerManager().SetTimer(CD_Handle, this, &ABuilding::EnableSpawning, CD_Time, false);
	SetActorTickEnabled(true);
	if (!CooldownWidgetInstance)
	{
		return;
	}
	CooldownWidgetInstance->SetActorHiddenInGame(false);
}

void ABuilding::SpawnInfoWidget()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = GetOwner();
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Location = InfoWidgetSpawnPoint->GetComponentLocation();
	InfoWidgetActorInst = GetWorld()->SpawnActor<AActor>(InfoWidgetActor, Location, Rotation, SpawnParameters);
	SetInfoWidgetStats(InfoWidgetActorInst);
	
	DisableInfoWidget();
}

void ABuilding::SetInfoWidgetStats(AActor* WidgetActor)
{
	UWidgetComponent* WidgetComponent = WidgetActor->FindComponentByClass<UWidgetComponent>();

	if (!WidgetComponent)
	{
		return;
	}
	
	// Unit name
	UTextBlock* TXT_Unit = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Unit"));
	if (TXT_Unit && BuildingDataAsset && BuildingDataAsset->UnitToSpawnData)
	{
		FName UnitFName = BuildingDataAsset->UnitToSpawnData->UnitName;
		FText UnitName = FText::FromName(UnitFName);
		TXT_Unit->SetText(UnitName);
	}

	// Unit Price
	UTextBlock* TXT_Price = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Price"));
	if (TXT_Price && BuildingDataAsset && BuildingDataAsset->UnitToSpawnData)
	{
		float UnitPrice = BuildingDataAsset->UnitToSpawnData->Price;
		FString UnitPriceString = FString::SanitizeFloat(UnitPrice);
		FText UnitPriceText = FText::FromString(UnitPriceString);
		TXT_Price->SetText(UnitPriceText);
	}

	// Unit Strenght
	UTextBlock* TXT_Strenght = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Strenght"));
	if (TXT_Strenght && BuildingDataAsset && BuildingDataAsset->UnitToSpawnData)
	{
		float UnitStrenght = BuildingDataAsset->UnitToSpawnData->Strength;
		FString UnitStrenghtString = FString::SanitizeFloat(UnitStrenght);
		FText UnitStrenghtText = FText::FromString(UnitStrenghtString);
		TXT_Strenght->SetText(UnitStrenghtText);
	}

	// Unit Speed
	UTextBlock* TXT_Speed = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Speed"));
	if (TXT_Speed && BuildingDataAsset && BuildingDataAsset->UnitToSpawnData)
	{
		float UnitSpeed = BuildingDataAsset->UnitToSpawnData->Speed;
		FString UnitSpeedString = FString::SanitizeFloat(UnitSpeed);
		FText UnitSpeedText = FText::FromString(UnitSpeedString);
		TXT_Speed->SetText(UnitSpeedText);
	}
}

FVector ABuilding::GetActorBoundingBoxExtent() const
{
	FBox ActorBounds = GetComponentsBoundingBox();
	FVector BoundingBoxExtent;
	BoundingBoxExtent = ActorBounds.GetExtent();
	return BoundingBoxExtent;
}

void ABuilding::EnableInfoWidget()
{
	InfoWidgetActorInst->SetActorHiddenInGame(false);
	InfoWidgetActorInst->SetActorTickEnabled(true);
	UWidgetComponent* WidgetComponent = InfoWidgetActorInst->FindComponentByClass<UWidgetComponent>();
	if (!WidgetComponent)
	{
		return;
	}
	
	WidgetComponent->PrimaryComponentTick.bCanEverTick = true;
	UTextBlock* TXT_Price = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Price"));
	UResourcesComponent* ResourcesComponent = GetOwner()->FindComponentByClass<UResourcesComponent>();
	if (!ResourcesComponent)
	{
		return;
	}
	
	if (ResourcesComponent->AvailableResources < BuildingDataAsset->UnitToSpawnData->Price)
	{
		FSlateColor Red = FColor::Red;
		TXT_Price->SetColorAndOpacity(Red);
	}
	else
	{
		FSlateColor Green = FColor::Green;
		TXT_Price->SetColorAndOpacity(Green);
	}
}

void ABuilding::SpawnCooldownWidget()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	FVector SpawnLoc = GetActorLocation() + FVector(0, 0, CooldownWidgetHeight);
	FRotator SpawnRot = FRotator::ZeroRotator;
	CooldownWidgetInstance = GetWorld()->SpawnActor<ACooldownActor>(CooldownWidgetClass, SpawnLoc, SpawnRot, SpawnParameters);
	
}

void ABuilding::DisableInfoWidget()
{
	InfoWidgetActorInst->SetActorHiddenInGame(true);
	InfoWidgetActorInst->SetActorTickEnabled(false);
	UWidgetComponent* WidgetComponent = InfoWidgetActorInst->FindComponentByClass<UWidgetComponent>();
	
	if (WidgetComponent)
	{
		WidgetComponent->PrimaryComponentTick.bCanEverTick = false;
	}
}

void ABuilding::SetOverlayMaterial()
{
	if (!OverlayMaterial)
	{
		return;
	}
	
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	for (UStaticMeshComponent* MeshComponent : MeshComponents)
	{
		if (!MeshComponent)
		{
			return;
		}
		
		for (int32 i = 0; i < MeshComponent->GetNumMaterials(); i++)
		{
			MeshComponent->SetMaterial(i, OverlayMaterial);
		}
	}
}

void ABuilding::RemoveOverlayMaterial()
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	for (UStaticMeshComponent* MeshComponent : MeshComponents)
	{
		if (!MeshComponent)
		{
			return;
		}
		
		for (int32 i = 0; i < MeshComponent->GetNumMaterials(); i++)
		{
			UMaterialInterface* DefaultMaterial = MeshComponent->GetStaticMesh()->GetMaterial(i);
			MeshComponent->SetMaterial(i, DefaultMaterial);
		}
	}
}


