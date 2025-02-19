#include "Crystal.h"
#include "BoxComponent.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"

ACrystal::ACrystal()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	SM_Crystal = CreateDefaultSubobject<UStaticMeshComponent>("CrystalBody");
	SM_Crystal->SetupAttachment(RootComponent);

	SM_Bottom = CreateDefaultSubobject<UStaticMeshComponent>("BottomMesh");
	SM_Bottom->SetupAttachment(RootComponent);

	// Collision Box
	CrystalBox = CreateDefaultSubobject<UBoxComponent>("CrystalBox");
	CrystalBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CrystalBox->SetCollisionObjectType(ECC_WorldDynamic);
	CrystalBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CrystalBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CrystalBox->SetupAttachment(RootComponent);
	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
}

void ACrystal::BeginPlay()
{
	Super::BeginPlay();
}

