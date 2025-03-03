#include "PowerUp.h"

APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Root);
}

void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

