#include "SpawnPointCrystal.h"

#include "ArrowComponent.h"
#include "BillboardComponent.h"

ASpawnPointCrystal::ASpawnPointCrystal(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"SpawnRoot");
	RootComponent = SpawnRoot;

	BillboardComponent = ObjectInitializer.CreateDefaultSubobject<UBillboardComponent>(this,"BillboardComponent");
	BillboardComponent->SetupAttachment(RootComponent);

	Arrow = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this,"Arrow");
	Arrow->ArrowSize = 0.1f;
	Arrow->ArrowColor = FColor::Blue;
	Arrow->SetupAttachment(RootComponent);
}

void ASpawnPointCrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

