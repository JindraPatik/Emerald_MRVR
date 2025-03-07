#include "ShieldActor.h"

#include "Emerald_MRVR/Actors/Units/Unit.h"


AShieldActor::AShieldActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = Body;
}

void AShieldActor::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AShieldActor::AShieldActor::OnOverlapped);

	/* Start upscaling Shield */
	bIsUpscaling = true;
}

void AShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsUpscaling)
	{
		UpScaleShield(DeltaTime);
	}
}

void AShieldActor::OnOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	AUnit* Unit = Cast<AUnit>(OtherActor);
	if (!Unit)
	{
		return;
	}
	if (Unit->GetOwner() != GetOwner())
	{
		Unit->KillMe();
	}
}


void AShieldActor::DestroyShield()
{
	Destroy();
}


void AShieldActor::UpScaleShield(float DeltaTime)
{
	if (ShieldRadius < ShieldMaxRadius)
	{
		ShieldRadius = FMath::Lerp(ShieldRadius, ShieldMaxRadius, DeltaTime);
		Body->SetWorldScale3D(FVector(1, 1, 1) * ShieldRadius);
	}
	else
	{
		bIsUpscaling = false;
	}
}



