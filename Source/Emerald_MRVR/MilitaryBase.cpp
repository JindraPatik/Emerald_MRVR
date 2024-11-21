

#include "MilitaryBase.h"
#include "BoxComponent.h"
#include "DebugMacros.h"
#include "Components/BuildingsModuleComponent.h"
#include "Components/DownScaleComponent.h"
#include "Net/UnrealNetwork.h"

#define DRAW_SPHERE (Location) if (GetWorld()) DrawDebugSphere()

AMilitaryBase::AMilitaryBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	RootComponent = BaseBody;
	
	BaseBox = CreateDefaultSubobject<UBoxComponent>("BaseBox");
	BaseBox->SetupAttachment(BaseBody);
	
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	BuildingsModuleComponent = CreateDefaultSubobject<UBuildingsModuleComponent>("BuildingsModuleComponent");
	
	SpawnPoint_Ground = CreateDefaultSubobject<USceneComponent>("SpawnPointGround");
	SpawnPoint_Ground->SetupAttachment(RootComponent);
	SpawnPoint_Air = CreateDefaultSubobject<USceneComponent>("SpawnPointAir");
	SpawnPoint_Air->SetupAttachment(RootComponent);

	// Modules
	Modules = CreateDefaultSubobject<USceneComponent>(TEXT("ModulesRoot"));
	Modules->SetupAttachment(RootComponent);

	// Mine
	Mine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mine"));
	Mine->SetupAttachment(Modules);
	MineBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MineBox"));
	MineBox->SetupAttachment(Mine);

	// Barracs
	Barracs = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barracs"));
	Barracs->SetupAttachment(Modules);
	BarracsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BarracsBox"));
	BarracsBox->SetupAttachment(Barracs);

	// Garage
	Garage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Garage"));
	Garage->SetupAttachment(Modules);
	GarageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GarageBox"));
	GarageBox->SetupAttachment(Garage);

	// Postupne doplnit vsechny moduly!!!

	
}

void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMilitaryBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Ground)
	DOREPLIFETIME(AMilitaryBase, SpawnPoint_Air)
}

void AMilitaryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
