#include "Projectile.h"
#include "Components/DownScaleComponent.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	BaseBody->SetupAttachment(RootComp);

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

