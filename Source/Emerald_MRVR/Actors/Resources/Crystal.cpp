#include "Crystal.h"
#include "BoxComponent.h"
#include "Concepts/Iterable.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"
#include "Trace/Detail/Transport.h"
#include "Widgets/Text/STextScroller.h"

ACrystal::ACrystal()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	CrystalBody = CreateDefaultSubobject<UStaticMeshComponent>("CrystalBody");
	CrystalBody->SetupAttachment(RootComponent);

	CrystalBottom = CreateDefaultSubobject<UStaticMeshComponent>("BottomMesh");
	CrystalBottom->SetupAttachment(RootComponent);

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
	AdjustLocationAfterSpawn();
}

/* Snap Crystal to ground after spawn */
void ACrystal::AdjustLocationAfterSpawn()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FHitResult OutHitResult;
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + (GetActorUpVector() * -50.f);
	bool bHit = World->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_WorldStatic);
	if (bHit)
	{
		SetActorLocation(OutHitResult.ImpactPoint);
	}
}



