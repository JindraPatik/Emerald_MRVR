#include "ShieldActor.h"

#include "Emerald_MRVR/Actors/Units/Unit.h"


AShieldActor::AShieldActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	
	Body = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,"Body");
	RootComponent = Body;
}

void AShieldActor::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AShieldActor::AShieldActor::OnOverlapped);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	World->GetTimerManager().SetTimer(ShieldDeactivationTimer, this, &AShieldActor::DeactivateShield, ShieldDuration);

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

	if (bIsDownscaling)
	{
		DownScaleShield(DeltaTime);
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


void AShieldActor::DeactivateShield()
{
	bIsUpscaling = false;
	bIsDownscaling = true;
}

void AShieldActor::UpScaleShield(float DeltaTime)
{
	if (ShieldRadius <= ShieldMaxRadius + KINDA_SMALL_NUMBER)
	{
		ShieldRadius = FMath::FInterpTo(ShieldRadius, ShieldMaxRadius, DeltaTime, ScalingMultiplicator);
		Body->SetWorldScale3D(FVector(1, 1, 1) * ShieldRadius);
	}
	else
	{
		bIsUpscaling = false;
	}
}

void AShieldActor::DownScaleShield(float DeltaTime)
{
	if (ShieldRadius > ShieldMinRadius)
	{
		ShieldRadius = FMath::FInterpTo(ShieldRadius, ShieldMinRadius, DeltaTime, ScalingMultiplicator);
		Body->SetWorldScale3D(FVector(1, 1, 1) * ShieldRadius);
		
		if (ShieldRadius <= ShieldMinRadius + KINDA_SMALL_NUMBER)
		{
			bIsDownscaling = false;
			Destroy(); 
		}
	}
}






