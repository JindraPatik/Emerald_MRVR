
#include "MilitaryStation.h"
#include "BoxComponent.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"
#include "Net/UnrealNetwork.h"

#define DRAW_SPHERE (Location) if (GetWorld()) DrawDebugSphere()

AMilitaryStation::AMilitaryStation()
{
	PrimaryActorTick.bCanEverTick = false;

	StationRoot = CreateDefaultSubobject<USceneComponent>("BaseRoot");
	SetRootComponent(StationRoot);

	Body_Root = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	Body_Root->SetupAttachment(StationRoot);
	
	BaseBox = CreateDefaultSubobject<UBoxComponent>("BaseBox");
	BaseBox->SetupAttachment(Body_Root);
	BaseBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BaseBox->SetCollisionObjectType(ECC_WorldDynamic);
	BaseBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	BaseBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	
	SpawnPointGround = CreateDefaultSubobject<USceneComponent>("SpawnPointGround");
	SpawnPointGround->SetupAttachment(StationRoot);
	SpawnPointAir = CreateDefaultSubobject<USceneComponent>("SpawnPointAir");
	SpawnPointAir->SetupAttachment(StationRoot);

	// Buildings Locations
	PositionsRoot = CreateDefaultSubobject<USceneComponent>("Positions_Root");
	PositionsRoot->SetupAttachment(StationRoot);
	
	MinePosition = CreateDefaultSubobject<USceneComponent>("MinePosition");
	MinePosition->ComponentTags.Add("Mine");
	BuildingPositions.Add(MinePosition);
	MinePosition->SetupAttachment(PositionsRoot);
	
	PrisonPosition = CreateDefaultSubobject<USceneComponent>("PrisonPosition");
	PrisonPosition->ComponentTags.Add("PrisonPosition");
	BuildingPositions.Add(PrisonPosition);
	PrisonPosition->SetupAttachment(PositionsRoot);
	
	BankPosition = CreateDefaultSubobject<USceneComponent>("BankPosition");
	BankPosition->ComponentTags.Add("Bank");
	BuildingPositions.Add(BankPosition);
	BankPosition->SetupAttachment(PositionsRoot);

	UVKSCPosition = CreateDefaultSubobject<USceneComponent>("UVKSCPosition");
	UVKSCPosition->ComponentTags.Add("UVKSC");
	BuildingPositions.Add(UVKSCPosition);
	UVKSCPosition->SetupAttachment(PositionsRoot);
	
	MicrosoftPosition = CreateDefaultSubobject<USceneComponent>("Microsoft");
	MicrosoftPosition->ComponentTags.Add("Microsoft");
	BuildingPositions.Add(MicrosoftPosition);
	MicrosoftPosition->SetupAttachment(PositionsRoot);

	BarracsPosition = CreateDefaultSubobject<USceneComponent>("BarracsPosition");
	BarracsPosition->ComponentTags.Add("Barracs");
	BuildingPositions.Add(BarracsPosition);
	BarracsPosition->SetupAttachment(PositionsRoot);
	
	GaragePosition = CreateDefaultSubobject<USceneComponent>("GaragePosition");
	GaragePosition->ComponentTags.Add("Garage");
	BuildingPositions.Add(GaragePosition);
	GaragePosition->SetupAttachment(PositionsRoot);

	LPlatformPosition = CreateDefaultSubobject<USceneComponent>("LPlatformPosition");
	LPlatformPosition->ComponentTags.Add("LPlatform");
	BuildingPositions.Add(LPlatformPosition);
	LPlatformPosition->SetupAttachment(PositionsRoot);

	MinecraftPosition = CreateDefaultSubobject<USceneComponent>("MinecraftPosition");
	MinecraftPosition->ComponentTags.Add("Minecraft");
	BuildingPositions.Add(MinecraftPosition);
	MinecraftPosition->SetupAttachment(PositionsRoot);

	HeliportPosition = CreateDefaultSubobject<USceneComponent>("HeliportPosition");
	HeliportPosition->ComponentTags.Add("Heliport");
	BuildingPositions.Add(HeliportPosition);
	HeliportPosition->SetupAttachment(PositionsRoot);
	
	RunwayPosition = CreateDefaultSubobject<USceneComponent>("Runway_Position");
	RunwayPosition->ComponentTags.Add("Runway");
	BuildingPositions.Add(RunwayPosition);
	RunwayPosition->SetupAttachment(PositionsRoot);
	
	AerialPosition = CreateDefaultSubobject<USceneComponent>("Aerial_Position");
	AerialPosition->ComponentTags.Add("Aerial");
	BuildingPositions.Add(AerialPosition);
	AerialPosition->SetupAttachment(PositionsRoot);

	HangarPosition = CreateDefaultSubobject<USceneComponent>("Hangar_Position");
	HangarPosition->ComponentTags.Add("Hangar");
	BuildingPositions.Add(HangarPosition);
	HangarPosition->SetupAttachment(PositionsRoot);

	RocketLauncherPosition = CreateDefaultSubobject<USceneComponent>("RocketLauncher_Position");
	RocketLauncherPosition->ComponentTags.Add("RocketLauncher");
	BuildingPositions.Add(RocketLauncherPosition);
	RocketLauncherPosition->SetupAttachment(PositionsRoot);
}

void AMilitaryStation::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMilitaryStation, VR_Pawn);
	DOREPLIFETIME(AMilitaryStation, Body_Root)
	DOREPLIFETIME(AMilitaryStation, SpawnPointGround)
	DOREPLIFETIME(AMilitaryStation, SpawnPointAir)
	DOREPLIFETIME(AMilitaryStation, ResourcesWidgetInstance);
	DOREPLIFETIME(AMilitaryStation, HealthWidgetInstance);
	DOREPLIFETIME(AMilitaryStation, OriginalMaterial);
	DOREPLIFETIME(AMilitaryStation, HoveredMaterial);
	DOREPLIFETIME(AMilitaryStation, StationRoot);

	// Modules
	DOREPLIFETIME(AMilitaryStation, MinePosition);
	DOREPLIFETIME(AMilitaryStation, PrisonPosition);
	DOREPLIFETIME(AMilitaryStation, BankPosition);
	DOREPLIFETIME(AMilitaryStation, BankPosition);
	DOREPLIFETIME(AMilitaryStation, UVKSCPosition);
	DOREPLIFETIME(AMilitaryStation, MicrosoftPosition);
	DOREPLIFETIME(AMilitaryStation, BarracsPosition);
	DOREPLIFETIME(AMilitaryStation, GaragePosition);
	DOREPLIFETIME(AMilitaryStation, LPlatformPosition);
	DOREPLIFETIME(AMilitaryStation, MinecraftPosition);
	DOREPLIFETIME(AMilitaryStation, HeliportPosition);
	DOREPLIFETIME(AMilitaryStation, RunwayPosition);
	DOREPLIFETIME(AMilitaryStation, AerialPosition);
	DOREPLIFETIME(AMilitaryStation, HangarPosition);
	DOREPLIFETIME(AMilitaryStation, RocketLauncherPosition);
	DOREPLIFETIME(AMilitaryStation, BuildingPositions);
}

void AMilitaryStation::BeginPlay()
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

void AMilitaryStation::SpawnResourcesWidget()
{
	if (!HasAuthority())
	{
		Server_SpawnResourcesWidget();
	}
	
	if (VR_Pawn && VR_Pawn->IsLocallyControlled())
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		FVector Location = GetActorLocation() + ResourcesWidgetOffset;
		ResourcesWidgetInstance = World->SpawnActor<AActor>(ResourcesWBP, Location, FRotator::ZeroRotator, SpawnParams);
	}
}

void AMilitaryStation::Server_SpawnResourcesWidget_Implementation()
{
	SpawnResourcesWidget();
}

void AMilitaryStation::SpawnHealthWidget()
{
	if (!HasAuthority())
	{
		Server_SpawnHealthWidget();
		return;
	}
	
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	FActorSpawnParameters SpawnParams;
	FVector Location = GetActorLocation() + HealthWidgetOffset;
	
	HealthWidgetInstance = World->SpawnActor<AActor>(HealthWidget, Location, FRotator::ZeroRotator, SpawnParams);
	if (!HealthWidgetInstance)
	{
		return;
	}
	HealthWidgetInstance->SetReplicates(true);
}

void AMilitaryStation::Server_SpawnHealthWidget_Implementation()
{
	SpawnHealthWidget();
}



