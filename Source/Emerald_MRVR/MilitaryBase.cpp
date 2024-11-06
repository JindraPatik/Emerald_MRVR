

#include "MilitaryBase.h"
#include "BoxComponent.h"
#include "Components/DownScaleComponent.h"


AMilitaryBase::AMilitaryBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	BaseBox = CreateDefaultSubobject<UBoxComponent>("BaseBox");
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
}

void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMilitaryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

