#include "Projectile.h"

#include "Unit.h"
#include "Components/DownScaleComponent.h"
#include "Components/UnitMovementComponent.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);

	MovementComponent = CreateDefaultSubobject<UUnitMovementComponent>("MovementComponent");

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	BaseBody->SetupAttachment(RootComponent);
	

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	BaseBody->SetSimulatePhysics(true);
	BaseBody->SetEnableGravity(true);
	// BaseBody->SetMassScale(NAME_None,0.000000001f);
}

void AProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	/*FVector AntiGravity = -GetWorld()->GetGravityZ() * BaseBody->GetMass() * FVector::UpVector * 0.88f; 
	BaseBody->AddForce(AntiGravity);*/

	
}

