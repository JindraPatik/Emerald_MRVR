#include "PowerUp.h"

#include "Emerald_MRVR/EmeraldKeeper.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"

APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	Body->SetupAttachment(Root);

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


void APowerUp::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	
}
