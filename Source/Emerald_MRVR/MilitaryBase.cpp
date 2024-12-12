
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
	BaseBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BaseBox->SetCollisionObjectType(ECC_WorldDynamic);
	BaseBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	BaseBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	
	SpawnPoint_Ground = CreateDefaultSubobject<USceneComponent>("SpawnPointGround");
	SpawnPoint_Ground->SetupAttachment(BaseBody);
	SpawnPoint_Air = CreateDefaultSubobject<USceneComponent>("SpawnPointAir");
	SpawnPoint_Air->SetupAttachment(BaseBody);

	// Modules Locations
	Mine_Position = CreateDefaultSubobject<USceneComponent>("MinePosition");
	Mine_Position->ComponentTags.Add("Mine");
	
	Prison_Position = CreateDefaultSubobject<USceneComponent>("PrisonPosition");
	Prison_Position->ComponentTags.Add("PrisonPosition");

	Bank_Position = CreateDefaultSubobject<USceneComponent>("BankPosition");
	Bank_Position->ComponentTags.Add("Bank");

	UVKSC_Position = CreateDefaultSubobject<USceneComponent>("UVKSCPosition");
	UVKSC_Position->ComponentTags.Add("UVKSC");

	Microsoft_Position = CreateDefaultSubobject<USceneComponent>("Microsoft");
	Microsoft_Position->ComponentTags.Add("Microsoft");

	Barracs_Position = CreateDefaultSubobject<USceneComponent>("BarracsPosition");
	Barracs_Position->ComponentTags.Add("Barracs");

	Garage_Position = CreateDefaultSubobject<USceneComponent>("GaragePosition");
	Garage_Position->ComponentTags.Add("Garage");

	LPlatform_Position = CreateDefaultSubobject<USceneComponent>("LPlatformPosition");
	LPlatform_Position->ComponentTags.Add("LPlatform");

	Minecraft_Position = CreateDefaultSubobject<USceneComponent>("MinecraftPosition");
	Minecraft_Position->ComponentTags.Add("Minecraft");

	Heliport_Position = CreateDefaultSubobject<USceneComponent>("HeliportPosition");
	Heliport_Position->ComponentTags.Add("Heliport");
	
	Runway_Position = CreateDefaultSubobject<USceneComponent>("Runway_Position");
	Runway_Position->ComponentTags.Add("Runway");
	
	Aerial_Position = CreateDefaultSubobject<USceneComponent>("Aerial_Position");
	Aerial_Position->ComponentTags.Add("Aerial");

	Hangar_Position = CreateDefaultSubobject<USceneComponent>("Hangar_Position");
	Hangar_Position->ComponentTags.Add("Hangar");

	RocketLauncher_Position = CreateDefaultSubobject<USceneComponent>("RocketLauncher_Position");
	RocketLauncher_Position->ComponentTags.Add("RocketLauncher");
}

void AMilitaryBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMilitaryBase, General);
	DOREPLIFETIME(AMilitaryBase, BaseBody)
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Ground)
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Air)
	DOREPLIFETIME(AMilitaryBase, ResourcesWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, HealthWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, OriginalMaterial);
	DOREPLIFETIME(AMilitaryBase, HoveredMaterial);
	
	DOREPLIFETIME(AMilitaryBase, Mine_Position);
	DOREPLIFETIME(AMilitaryBase, Prison_Position);
	DOREPLIFETIME(AMilitaryBase, Bank_Position);
	DOREPLIFETIME(AMilitaryBase, Bank_Position);
	DOREPLIFETIME(AMilitaryBase, UVKSC_Position);
	DOREPLIFETIME(AMilitaryBase, Microsoft_Position);
	DOREPLIFETIME(AMilitaryBase, Barracs_Position);
	DOREPLIFETIME(AMilitaryBase, Garage_Position);
	DOREPLIFETIME(AMilitaryBase, LPlatform_Position);
	DOREPLIFETIME(AMilitaryBase, Minecraft_Position);
	DOREPLIFETIME(AMilitaryBase, Heliport_Position);
	DOREPLIFETIME(AMilitaryBase, Runway_Position);
	DOREPLIFETIME(AMilitaryBase, Aerial_Position);
	DOREPLIFETIME(AMilitaryBase, Hangar_Position);
	DOREPLIFETIME(AMilitaryBase, RocketLauncher_Position);

	
	
}

void AMilitaryBase::PostInitProperties()
{
	Super::PostInitProperties();
}

void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
	General = Cast<AMR_General>(GetOwner());
	
	if (ensure(General))
	{
		SpawnResourcesWidget();
		if (General->IsLocallyControlled())
		{
			SpawnHealthWidget();
		}
		
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
		return;
	}
}

void AMilitaryBase::Server_SpawnHealthWidget_Implementation()
{
	SpawnHealthWidget();
}



