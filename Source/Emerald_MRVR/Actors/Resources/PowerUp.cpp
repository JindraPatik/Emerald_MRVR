#include "PowerUp.h"

#include "Emerald_MRVR/EmeraldKeeper.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"

APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = Body;
	Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Body->SetCollisionResponseToAllChannels(ECR_Ignore);
	Body->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Body->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Body->SetGenerateOverlapEvents(true);
	// Body->SetSimulatePhysics(true);

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownScaleComponent");
	DownScaleComponent->DownscaleFactor = GLOBAL_DOWNSCALE_VALUE;
}

void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &APowerUp::OnOverlap);

}

void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/* Implement functionality of PowerUps in Children */
void APowerUp::Activate()
{
	
}

void APowerUp::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Body->SetSimulatePhysics(false);
	UE_LOG(LogTemp, Warning, TEXT("APowerUp::OnOverlap"));
}



