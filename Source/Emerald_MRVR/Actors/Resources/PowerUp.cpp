#include "PowerUp.h"

#include "Emerald_MRVR/Actors/Units/SpecialUnits/Picker.h"

APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	Body->SetupAttachment(Root);
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
