#include "WorldWrapper.h"

#include "Emerald_MRVR/EmeraldKeeper.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"

AWorldWrapper::AWorldWrapper()
{
	PrimaryActorTick.bCanEverTick = true;

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownScaleComponent");
}

void AWorldWrapper::BeginPlay()
{
	Super::BeginPlay();
	DownScaleComponent->DownscaleFactor = GLOBAL_DOWNSCALE_VALUE;
	
}

