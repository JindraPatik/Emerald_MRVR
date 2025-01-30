#include "Projectile.h"
#include "Components/DownScaleComponent.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	BaseBody->SetupAttachment(RootComponent);
	

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	BaseBody->SetSimulatePhysics(true);
	BaseBody->SetEnableGravity(true);
	FVector ImpulseVector = GetOwner()->GetActorForwardVector() * ProjectileImpulseMultiplier;
	BaseBody->AddImpulse(ImpulseVector);
}

