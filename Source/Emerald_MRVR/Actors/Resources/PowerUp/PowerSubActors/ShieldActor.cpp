#include "ShieldActor.h"


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
	/* Destroy Enemy Units */
}


void AShieldActor::DestroyShield()
{
	Destroy();
}



