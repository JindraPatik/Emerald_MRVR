#include "SpawnPointCrystal.h"

#include "ArrowComponent.h"
#include "BillboardComponent.h"

ASpawnPointCrystal::ASpawnPointCrystal()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnRoot = CreateDefaultSubobject<USceneComponent>("SpawnRoot");
	RootComponent = SpawnRoot;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
	BillboardComponent->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->ArrowSize = 0.1f;
	Arrow->ArrowColor = FColor::Blue;
	Arrow->SetupAttachment(RootComponent);
}

void ASpawnPointCrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

