#include "ModuleActor.h"
#include "DebugMacros.h"
#include "Blueprint/WidgetTree.h"
#include "Components/DownScaleComponent.h"
#include "Components/ResourcesComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "CORE/MR_General.h"
#include "Data/BuildingDataAsset.h"
#include "Data/UnitDataAsset.h"
#include "Net/UnrealNetwork.h"

AModuleActor::AModuleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	ModuleMeshRoot = CreateDefaultSubobject<UStaticMeshComponent>("ModuleBody");
	ModuleMeshRoot->SetupAttachment(SceneRoot);
}

void AModuleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AModuleActor, ModuleMeshRoot);
	DOREPLIFETIME(AModuleActor, BuildingDataAsset);
	DOREPLIFETIME(AModuleActor, SceneRoot);
}

void AModuleActor::BeginPlay()
{
	Super::BeginPlay();
}

void AModuleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AModuleActor::SpawnInfoWidget()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = GetOwner();
;	FRotator Rotation = FRotator::ZeroRotator;
	FVector Location = GetActorLocation() + FVector(0.f, 0.f, InfoWidgetHeight);
	InfoWidgetActorInst = GetWorld()->SpawnActor<AActor>(InfoWidgetActor, Location, Rotation, SpawnParameters);
	SetInfoWidgetStats(InfoWidgetActorInst);
	
	DisableInfoWidget();
}

void AModuleActor::SetInfoWidgetStats(AActor* WidgetActor)
{
	UWidgetComponent* WidgetComponent = WidgetActor->FindComponentByClass<UWidgetComponent>();
	if (WidgetComponent)
	{
		UTextBlock* TXT_Unit = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Unit"));
		if (TXT_Unit && BuildingDataAsset && BuildingDataAsset->UnitToSpawnData)
		{
			FName UnitFName = BuildingDataAsset->UnitToSpawnData->UnitName;
			FText UnitName = FText::FromName(UnitFName);
			TXT_Unit->SetText(UnitName);
		}
		
		UTextBlock* TXT_Price = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Price"));
		if (TXT_Price && BuildingDataAsset && BuildingDataAsset->UnitToSpawnData)
		{
			float UnitPrice = BuildingDataAsset->UnitToSpawnData->Price;
			FString UnitPriceString = FString::SanitizeFloat(UnitPrice);
			FText UnitPriceText = FText::FromString(UnitPriceString);
			TXT_Price->SetText(UnitPriceText);
		}
		
		UTextBlock* TXT_Strenght = Cast<UTextBlock>(WidgetComponent->GetWidget()->WidgetTree->FindWidget("TXT_Strenght"));
		if (TXT_Strenght && BuildingDataAsset && BuildingDataAsset->UnitToSpawnData)
		{
			float UnitStrenght = BuildingDataAsset->UnitToSpawnData->Strength;
			FString UnitStrenghtString = FString::SanitizeFloat(UnitStrenght);
			FText UnitStrenghtText = FText::FromString(UnitStrenghtString);
			TXT_Strenght->SetText(UnitStrenghtText);
		}

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

void AModuleActor::EnableInfoWidget()
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

void AModuleActor::DisableInfoWidget()
{
	InfoWidgetActorInst->SetActorHiddenInGame(true);
	InfoWidgetActorInst->SetActorTickEnabled(false);
	UWidgetComponent* WidgetComponent = InfoWidgetActorInst->FindComponentByClass<UWidgetComponent>();
	if (WidgetComponent)
	{
		WidgetComponent->PrimaryComponentTick.bCanEverTick = false;
	}
}

