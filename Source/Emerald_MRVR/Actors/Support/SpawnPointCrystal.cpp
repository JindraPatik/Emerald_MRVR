#include "SpawnPointCrystal.h"

#include "ArrowComponent.h"
#include "BillboardComponent.h"

ASpawnPointCrystal::ASpawnPointCrystal()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnRoot = CreateDefaultSubobject<USceneComponent>("SpawnRoot");
	RootComponent = SpawnRoot;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
	BillboardComponent->SetupAttachment(SpawnRoot);

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(SpawnRoot);
}

void ASpawnPointCrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

