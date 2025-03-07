#include "PowerUp.h"

#include "EngineUtils.h"
#include "SplineComponent.h"
#include "Emerald_MRVR/EmeraldKeeper.h"
#include "Emerald_MRVR/Actors/Support/PathPoint.h"
#include "Emerald_MRVR/Actors/Support/SpawnPointCrystal.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"
#include "Emerald_MRVR/Support/EmeraldBlueprintFunctionLibrary.h"

class APathPoint;
class ASpawnPointCrystal;

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
	
	FindLandingPoint();
	bIsFalling = true;
}

void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFalling)
	{
		Falling(DeltaTime);
	}
}

/* Implement functionality of PowerUps in Children */
void APowerUp::Activate()
{
	UE_LOG(LogTemp, Display, TEXT("APowerUp::Activate"));
}

void APowerUp::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("APowerUp::OnOverlap"));
}

void APowerUp::FindLandingPoint()
{
	TArray<ASpawnPointCrystal*> SpawnPointCrystals;
	for (TActorIterator<ASpawnPointCrystal> It(GetWorld()); It; ++It)
	{
		SpawnPointCrystals.Add(*It);
	}
	
	Algo::Sort(SpawnPointCrystals, [](const ASpawnPointCrystal* A, const ASpawnPointCrystal* B) {
		return A->index < B->index;
	});

	if (SpawnPointCrystals.Num() < 2)
	{
		UE_LOG(LogTemp, Error, TEXT("PowerUpSpawner: Not enough CrystalBoundry points found!"));
		return;
	}
	
	FVector SpawnPoint1 = SpawnPointCrystals[0]->GetActorLocation();
	FVector SpawnPoint2 = SpawnPointCrystals[1]->GetActorLocation();

	if (!PathPointClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerUp: Path point class not set in Editor!"))
		return;
	}

	TArray<APathPoint*> SplinePoints = UEmeraldBlueprintFunctionLibrary::SortPathPoints(this, PathPointClass, true);

	if (SplinePoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No PathPoints found for Landing spline."));
	}
	
	LandingSpline = UEmeraldBlueprintFunctionLibrary::CreateSplinePath(this, SpawnPoint1, SpawnPoint2, SplinePoints, this);

	if (!LandingSpline)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create Landing Spline!"));
		return;
	}

	float SplineDistance = FMath::RandRange(0.f, LandingSpline->GetSplineLength());
	LandingPoint = LandingSpline->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);
}


void APowerUp::Falling(float DeltaTime)
{
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), LandingPoint, DeltaTime, FallingSpeed);
	SetActorLocation(NewLocation);

	if (GetActorLocation() == LandingPoint + KINDA_SMALL_NUMBER)
	{
		bIsFalling = false;
		UE_LOG(LogTemp, Warning, TEXT("PowerUp Falling Stopped"));
	}
}





