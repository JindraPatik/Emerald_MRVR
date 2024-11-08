#include "Crystal.h"

#include "BoxComponent.h"
#include "Components/DownScaleComponent.h"


ACrystal::ACrystal()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	SM_Crystal = CreateDefaultSubobject<UStaticMeshComponent>("CrystalBody");
	SM_Crystal->SetupAttachment(RootComponent);

	SM_Bottom = CreateDefaultSubobject<UStaticMeshComponent>("BottomMesh");
	SM_Bottom->SetupAttachment(RootComponent);

	CrystalBox = CreateDefaultSubobject<UBoxComponent>("CrystalBox");

	CrystalBox->SetupAttachment(RootComponent);
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
}

void ACrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

