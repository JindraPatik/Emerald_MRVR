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

	Body->SetWorldScale3D(FVector(1, 1, 1) * ShieldRadius);
	
}


void AShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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



