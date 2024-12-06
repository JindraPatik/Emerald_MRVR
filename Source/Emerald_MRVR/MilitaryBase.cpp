
#include "MilitaryBase.h"
#include "BoxComponent.h"
#include "DebugMacros.h"
#include "Components/DownScaleComponent.h"
#include "CORE/MR_General.h"
#include "Net/UnrealNetwork.h"

#define DRAW_SPHERE (Location) if (GetWorld()) DrawDebugSphere()


AMilitaryBase::AMilitaryBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	SetRootComponent(BaseBody);
	SetReplicates(true);
	
	BaseBox = CreateDefaultSubobject<UBoxComponent>("BaseBox");
	BaseBox->SetupAttachment(BaseBody);
	
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	
	SpawnPoint_Ground = CreateDefaultSubobject<USceneComponent>("SpawnPointGround");
	SpawnPoint_Ground->SetupAttachment(BaseBody);
	SpawnPoint_Air = CreateDefaultSubobject<USceneComponent>("SpawnPointAir");
	SpawnPoint_Air->SetupAttachment(BaseBody);
}

void AMilitaryBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMilitaryBase, General);
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Ground)
	DOREPLIFETIME(AMilitaryBase, BaseBody)
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Air)
	DOREPLIFETIME(AMilitaryBase, ResourcesWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, HealthWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, OriginalMaterial);
	DOREPLIFETIME(AMilitaryBase, HoveredMaterial);
	
}

void AMilitaryBase::PostInitProperties()
{
	Super::PostInitProperties();
}

void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
	General = Cast<AMR_General>(GetOwner());
	DBG_ONE_PARAM(5, "general: %s", *General->GetName())
	
	if (ensure(General))
	{
		if (HealthWidgetInstance && ResourcesWidgetInstance)
		{
			General->bGameInitialized = true;
		}
	} 
}

void AMilitaryBase::SpawnResourcesWidget()
{
	if (!HasAuthority())
	{
		Server_SpawnResourcesWidget();
	}
	
	if (General && General->IsLocallyControlled())
	{
		TObjectPtr<UWorld> World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			FVector Location = GetActorLocation() + FVector(0.f, 0.f, 100.f);
			ResourcesWidgetInstance = World->SpawnActor<AActor>(ResourcesWBP, Location, FRotator::ZeroRotator, SpawnParams);
		}
	}
}

void AMilitaryBase::Server_SpawnResourcesWidget_Implementation()
{
	SpawnResourcesWidget();
}

void AMilitaryBase::SpawnHealthWidget()
{
	if (!HasAuthority())
	{
		Server_SpawnHealthWidget();
		return;
	}
	
	TObjectPtr<UWorld> World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FVector Location = GetActorLocation() + FVector(0.f, 0.f, 110.f);
		HealthWidgetInstance = World->SpawnActor<AActor>(HealthWidget, Location, FRotator::ZeroRotator, SpawnParams);
		if (HealthWidgetInstance)
		{
			HealthWidgetInstance->SetReplicates(true);
		}
	}
}

void AMilitaryBase::Server_SpawnHealthWidget_Implementation()
{
	SpawnHealthWidget();
}



