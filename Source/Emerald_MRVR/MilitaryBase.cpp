

#include "MilitaryBase.h"
#include "BoxComponent.h"
#include "DebugMacros.h"
#include "Components/BuildingsModuleComponent.h"
#include "Components/DownScaleComponent.h"


AMilitaryBase::AMilitaryBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	SetRootComponent(BaseBody);
	BaseBox = CreateDefaultSubobject<UBoxComponent>("BaseBox");
	BaseBox->SetupAttachment(RootComponent);
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	BuildingsModuleComponent = CreateDefaultSubobject<UBuildingsModuleComponent>("BuildingsModuleComponent");
	
}

void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMilitaryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

