#include "Projectile.h"

#include "BoxComponent.h"
#include "Unit.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	BaseBody->SetupAttachment(RootComponent);

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("CapsuleComponent");
	BoxComponent->SetupAttachment(BaseBody);
	
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->IgnoreActorWhenMoving(GetOwner(), true);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBoxOverlapped);
	
	BaseBody->SetSimulatePhysics(true);
	BaseBody->SetEnableGravity(true);
	FVector ImpulseVector = GetOwner()->GetActorForwardVector() * ProjectileImpulseMultiplier;
	BaseBody->AddImpulse(ImpulseVector);

	
}

void AProjectile::OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner()->GetOwner())
	{
		AUnit* HittedUnit = Cast<AUnit>(OtherActor);
		if (HittedUnit)
		{
			HittedUnit->Destroy();
		}

		// Add Explosion Particles
		GetOwner()->Destroy();
	}
	
}

