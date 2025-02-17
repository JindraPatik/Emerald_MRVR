#include "Building.h"
#include "Blueprint/WidgetTree.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"
#include "Components/ProgressBar.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Emerald_MRVR/Widgets/CooldownActor.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	ModuleMeshRoot = CreateDefaultSubobject<UStaticMeshComponent>("ModuleBody");
	ModuleMeshRoot->SetupAttachment(SceneRoot);

	InfoWidgetSpawnPoint = CreateDefaultSubobject<USceneComponent>("InfowidgetSpawnPoint");
	InfoWidgetSpawnPoint->SetupAttachment(RootComponent);
}

void ABuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuilding, ModuleMeshRoot);
	DOREPLIFETIME(ABuilding, BuildingDataAsset);
	DOREPLIFETIME(ABuilding, SceneRoot);
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();
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
		if (WidgetComp)
		{
			UUserWidget* UserWidget = Cast<UUserWidget>(WidgetComp->GetWidget());
			if (UserWidget)
			{
				UProgressBar* CooldownProgress = Cast<UProgressBar>(UserWidget->WidgetTree->FindWidget("Progress_CoolDown"));
				if (CooldownProgress)
				{
					CooldownProgress->SetPercent(Progress);
				}
			}
		}
		if (ElapsedTime >= CooldownDuration)
		{
			CooldownWidgetInstance->SetActorHiddenInGame(true);
			SetActorTickEnabled(false);
		}
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
	if (CooldownWidgetInstance)
	{
		CooldownWidgetInstance->SetActorHiddenInGame(false);
	}
}

void ABuilding::SpawnInfoWidget()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = GetOwner();
	FRotator Rotation = FRotator::ZeroRotator;
	//FVector Location = GetActorLocation() + FVector(0.f, 0.f, InfoWidgetHeight);
	FVector Location = InfoWidgetSpawnPoint->GetComponentLocation();
	InfoWidgetActorInst = GetWorld()->SpawnActor<AActor>(InfoWidgetActor, Location, Rotation, SpawnParameters);
	SetInfoWidgetStats(InfoWidgetActorInst);
	
	DisableInfoWidget();
}

void ABuilding::SetInfoWidgetStats(AActor* WidgetActor)
{
	UWidgetComponent* WidgetComponent = WidgetActor->FindComponentByClass<UWidgetComponent>();

	//pb: v pripade delsich bloku uvnitr if { } je vhodnejsi podminku udelat s opacnou logikou a blok pak nemusi byt uvnitr { }, je to prehlednejsi. V tomto pripade bych to zmenil na:
	//if (!WidgetComponent)		<<<
	//	return;
	//a zbytek muze zustat bez indentace v ramci tela funkce
	//UTextBlock* TXT_Unit = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Unit"));
	//... atd.

	if (WidgetComponent)
	{
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
}

void ABuilding::EnableInfoWidget()
{
	InfoWidgetActorInst->SetActorHiddenInGame(false);
	InfoWidgetActorInst->SetActorTickEnabled(true);
	UWidgetComponent* WidgetComponent = InfoWidgetActorInst->FindComponentByClass<UWidgetComponent>();
	if (WidgetComponent)
	{
		WidgetComponent->PrimaryComponentTick.bCanEverTick = true;
		UTextBlock* TXT_Price = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Price"));
		UResourcesComponent* ResourcesComponent = GetOwner()->FindComponentByClass<UResourcesComponent>();
		if (ResourcesComponent)
		{
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
	//pb: jeno?�dkov� podm�nky je lep�� ned�vat na jeden ?�dek, kv?li breakpoint?m, viz. doc
	if (!OverlayMaterial) return;

	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	for (UStaticMeshComponent* MeshComponent : MeshComponents)
	{
		if (MeshComponent)
		{
			for (int32 i = 0; i < MeshComponent->GetNumMaterials(); i++)
			{
				MeshComponent->SetMaterial(i, OverlayMaterial);
			}
		}
	}
}

void ABuilding::RemoveOverlayMaterial()
{
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents<UStaticMeshComponent>(MeshComponents);

	for (UStaticMeshComponent* MeshComponent : MeshComponents)
	{
		if (MeshComponent)
		{
			for (int32 i = 0; i < MeshComponent->GetNumMaterials(); i++)
			{
				UMaterialInterface* DefaultMaterial = MeshComponent->GetStaticMesh()->GetMaterial(i);
				MeshComponent->SetMaterial(i, DefaultMaterial);
			}
		}
	}
}


