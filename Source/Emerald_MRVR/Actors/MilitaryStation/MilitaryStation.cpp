
#include "MilitaryStation.h"
#include "BoxComponent.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"
#include "Net/UnrealNetwork.h"

#define DRAW_SPHERE (Location) if (GetWorld()) DrawDebugSphere()

AMilitaryStation::AMilitaryStation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	StationRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"BaseRoot");
	SetRootComponent(StationRoot);

	Body_Root = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,"BaseBody");
	Body_Root->SetupAttachment(StationRoot);
	
	BaseBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this,"BaseBox");
	BaseBox->SetupAttachment(Body_Root);
	BaseBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BaseBox->SetCollisionObjectType(ECC_WorldDynamic);
	BaseBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	BaseBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	DownScaleComponent = ObjectInitializer.CreateDefaultSubobject<UDownScaleComponent>(this,"DownscaleComponent");
	
	SpawnPointGround = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"SpawnPointGround");
	SpawnPointGround->SetupAttachment(StationRoot);
	SpawnPointAir = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"SpawnPointAir");
	SpawnPointAir->SetupAttachment(StationRoot);

	// Buildings Locations
	PositionsRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"Positions_Root");
	PositionsRoot->SetupAttachment(StationRoot);
	
	MinePosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"MinePosition");
	MinePosition->ComponentTags.Add("Mine");
	BuildingPositions.Add(MinePosition);
	MinePosition->SetupAttachment(PositionsRoot);
	
	PrisonPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"PrisonPosition");
	PrisonPosition->ComponentTags.Add("PrisonPosition");
	BuildingPositions.Add(PrisonPosition);
	PrisonPosition->SetupAttachment(PositionsRoot);
	
	BankPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"BankPosition");
	BankPosition->ComponentTags.Add("Bank");
	BuildingPositions.Add(BankPosition);
	BankPosition->SetupAttachment(PositionsRoot);

	UVKSCPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"UVKSCPosition");
	UVKSCPosition->ComponentTags.Add("UVKSC");
	BuildingPositions.Add(UVKSCPosition);
	UVKSCPosition->SetupAttachment(PositionsRoot);
	
	MicrosoftPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"Microsoft");
	MicrosoftPosition->ComponentTags.Add("Microsoft");
	BuildingPositions.Add(MicrosoftPosition);
	MicrosoftPosition->SetupAttachment(PositionsRoot);

	BarracsPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"BarracsPosition");
	BarracsPosition->ComponentTags.Add("Barracs");
	BuildingPositions.Add(BarracsPosition);
	BarracsPosition->SetupAttachment(PositionsRoot);
	
	GaragePosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"GaragePosition");
	GaragePosition->ComponentTags.Add("Garage");
	BuildingPositions.Add(GaragePosition);
	GaragePosition->SetupAttachment(PositionsRoot);

	LPlatformPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"LPlatformPosition");
	LPlatformPosition->ComponentTags.Add("LPlatform");
	BuildingPositions.Add(LPlatformPosition);
	LPlatformPosition->SetupAttachment(PositionsRoot);

	MinecraftPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"MinecraftPosition");
	MinecraftPosition->ComponentTags.Add("Minecraft");
	BuildingPositions.Add(MinecraftPosition);
	MinecraftPosition->SetupAttachment(PositionsRoot);

	HeliportPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"HeliportPosition");
	HeliportPosition->ComponentTags.Add("Heliport");
	BuildingPositions.Add(HeliportPosition);
	HeliportPosition->SetupAttachment(PositionsRoot);
	
	RunwayPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"Runway_Position");
	RunwayPosition->ComponentTags.Add("Runway");
	BuildingPositions.Add(RunwayPosition);
	RunwayPosition->SetupAttachment(PositionsRoot);
	
	AerialPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"Aerial_Position");
	AerialPosition->ComponentTags.Add("Aerial");
	BuildingPositions.Add(AerialPosition);
	AerialPosition->SetupAttachment(PositionsRoot);

	HangarPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"Hangar_Position");
	HangarPosition->ComponentTags.Add("Hangar");
	BuildingPositions.Add(HangarPosition);
	HangarPosition->SetupAttachment(PositionsRoot);

	RocketLauncherPosition = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"RocketLauncher_Position");
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



