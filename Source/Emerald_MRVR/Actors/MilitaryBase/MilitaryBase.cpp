
#include "MilitaryBase.h"
#include "BoxComponent.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"
#include "Net/UnrealNetwork.h"

#define DRAW_SPHERE (Location) if (GetWorld()) DrawDebugSphere()

AMilitaryBase::AMilitaryBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicates(true);

	BaseRoot = CreateDefaultSubobject<USceneComponent>("BaseRoot");
	SetRootComponent(BaseRoot);

	Body_Root = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	Body_Root->SetupAttachment(BaseRoot);
	
	BaseBox = CreateDefaultSubobject<UBoxComponent>("BaseBox");
	BaseBox->SetupAttachment(Body_Root);
	BaseBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BaseBox->SetCollisionObjectType(ECC_WorldDynamic);
	BaseBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	BaseBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	
	SpawnPoint_Ground = CreateDefaultSubobject<USceneComponent>("SpawnPointGround");
	SpawnPoint_Ground->SetupAttachment(BaseRoot);
	SpawnPoint_Air = CreateDefaultSubobject<USceneComponent>("SpawnPointAir");
	SpawnPoint_Air->SetupAttachment(BaseRoot);

	// Buildings Locations
	Positions_Root = CreateDefaultSubobject<USceneComponent>("Positions_Root");
	Positions_Root->SetupAttachment(BaseRoot);
	
	Mine_Position = CreateDefaultSubobject<USceneComponent>("MinePosition");
	Mine_Position->ComponentTags.Add("Mine");
	BuildingPositions.Add(Mine_Position);
	Mine_Position->SetupAttachment(Positions_Root);
	
	Prison_Position = CreateDefaultSubobject<USceneComponent>("PrisonPosition");
	Prison_Position->ComponentTags.Add("PrisonPosition");
	BuildingPositions.Add(Prison_Position);
	Prison_Position->SetupAttachment(Positions_Root);
	
	Bank_Position = CreateDefaultSubobject<USceneComponent>("BankPosition");
	Bank_Position->ComponentTags.Add("Bank");
	BuildingPositions.Add(Bank_Position);
	Bank_Position->SetupAttachment(Positions_Root);

	UVKSC_Position = CreateDefaultSubobject<USceneComponent>("UVKSCPosition");
	UVKSC_Position->ComponentTags.Add("UVKSC");
	BuildingPositions.Add(UVKSC_Position);
	UVKSC_Position->SetupAttachment(Positions_Root);
	
	Microsoft_Position = CreateDefaultSubobject<USceneComponent>("Microsoft");
	Microsoft_Position->ComponentTags.Add("Microsoft");
	BuildingPositions.Add(Microsoft_Position);
	Microsoft_Position->SetupAttachment(Positions_Root);

	Barracs_Position = CreateDefaultSubobject<USceneComponent>("BarracsPosition");
	Barracs_Position->ComponentTags.Add("Barracs");
	BuildingPositions.Add(Barracs_Position);
	Barracs_Position->SetupAttachment(Positions_Root);
	
	Garage_Position = CreateDefaultSubobject<USceneComponent>("GaragePosition");
	Garage_Position->ComponentTags.Add("Garage");
	BuildingPositions.Add(Garage_Position);
	Garage_Position->SetupAttachment(Positions_Root);

	LPlatform_Position = CreateDefaultSubobject<USceneComponent>("LPlatformPosition");
	LPlatform_Position->ComponentTags.Add("LPlatform");
	BuildingPositions.Add(LPlatform_Position);
	LPlatform_Position->SetupAttachment(Positions_Root);

	Minecraft_Position = CreateDefaultSubobject<USceneComponent>("MinecraftPosition");
	Minecraft_Position->ComponentTags.Add("Minecraft");
	BuildingPositions.Add(Minecraft_Position);
	Minecraft_Position->SetupAttachment(Positions_Root);

	Heliport_Position = CreateDefaultSubobject<USceneComponent>("HeliportPosition");
	Heliport_Position->ComponentTags.Add("Heliport");
	BuildingPositions.Add(Heliport_Position);
	Heliport_Position->SetupAttachment(Positions_Root);
	
	Runway_Position = CreateDefaultSubobject<USceneComponent>("Runway_Position");
	Runway_Position->ComponentTags.Add("Runway");
	BuildingPositions.Add(Runway_Position);
	Runway_Position->SetupAttachment(Positions_Root);
	
	Aerial_Position = CreateDefaultSubobject<USceneComponent>("Aerial_Position");
	Aerial_Position->ComponentTags.Add("Aerial");
	BuildingPositions.Add(Aerial_Position);
	Aerial_Position->SetupAttachment(Positions_Root);

	Hangar_Position = CreateDefaultSubobject<USceneComponent>("Hangar_Position");
	Hangar_Position->ComponentTags.Add("Hangar");
	BuildingPositions.Add(Hangar_Position);
	Hangar_Position->SetupAttachment(Positions_Root);

	RocketLauncher_Position = CreateDefaultSubobject<USceneComponent>("RocketLauncher_Position");
	RocketLauncher_Position->ComponentTags.Add("RocketLauncher");
	BuildingPositions.Add(RocketLauncher_Position);
	RocketLauncher_Position->SetupAttachment(Positions_Root);
}

void AMilitaryBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMilitaryBase, VR_Pawn);
	DOREPLIFETIME(AMilitaryBase, Body_Root)
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Ground)
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Air)
	DOREPLIFETIME(AMilitaryBase, ResourcesWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, HealthWidgetInstance);
	DOREPLIFETIME(AMilitaryBase, OriginalMaterial);
	DOREPLIFETIME(AMilitaryBase, HoveredMaterial);
	DOREPLIFETIME(AMilitaryBase, BaseRoot);

	// Modules
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
	DOREPLIFETIME(AMilitaryBase, BuildingPositions);
}

void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
	VR_Pawn = Cast<APawn>(GetOwner());

	// Initialize widgets
	if (ensure(VR_Pawn))
	{
		SpawnResourcesWidget();
		SpawnHealthWidget();
	} 
}

void AMilitaryBase::SpawnResourcesWidget()
{
	if (!HasAuthority())
	{
		Server_SpawnResourcesWidget();
	}
	
	if (VR_Pawn && VR_Pawn->IsLocallyControlled())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			FVector Location = GetActorLocation() + ResourcesWidgetOffset;
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
	
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FVector Location = GetActorLocation() + HealthWidgetOffset;
		
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



