#include "CrystalSpawnerComp.h"
#include "EngineUtils.h"
#include "SplineComponent.h"
#include "Emerald_MRVR/Actors/Resources/Crystal.h"
#include "Emerald_MRVR/Actors/Support/SpawnPointCrystal.h"
#include "Emerald_MRVR/CORE/GameModes/GameModeCommon.h"
#include "Emerald_MRVR/Support/EmeraldBlueprintFunctionLibrary.h"
#include "Engine/TargetPoint.h"


UCrystalSpawnerComp::UCrystalSpawnerComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UCrystalSpawnerComp::BeginPlay()
{
	Super::BeginPlay();
	AGameModeCommon* GM = Cast<AGameModeCommon>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->OnGameEndedDelegate.AddDynamic(this, &UCrystalSpawnerComp::StopSpawning);
	}
}

void UCrystalSpawnerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCrystalSpawnerComp::SpawnCrystal()
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
		UE_LOG(LogTemp, Error, TEXT("Not enough CrystalBoundry points found!"));
		return;
	}
	
	FVector SpawnPoint1 = SpawnPointCrystals[0]->GetActorLocation();
	FVector SpawnPoint2 = SpawnPointCrystals[1]->GetActorLocation();

	// Bacha, bIsreversed asi záleží na načtení pole bodů v levlu
	TArray<APathPoint*> SplinePoints = UEmeraldBlueprintFunctionLibrary::SortPathPoints(this, PathPointClass, true);

	if (SplinePoints.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No PathPoints found for Crystal spline."));
	}
	
	USplineComponent* CrystalSpline = UEmeraldBlueprintFunctionLibrary::CreateSplinePath(this, SpawnPoint1, SpawnPoint2, SplinePoints, GetOwner());

	if (!CrystalSpline)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create Crystal Spline!"));
		return;
	}

	float SplineDistance = FMath::RandRange(0.f, CrystalSpline->GetSplineLength());
	FVector CrystalSpawnLocation = CrystalSpline->GetLocationAtDistanceAlongSpline(SplineDistance, ESplineCoordinateSpace::World);
	
    FRotator SpawnRotation = FRotator(0.f,FMath::RandRange(0.f, 360.f), 0.f);
    FActorSpawnParameters SpawnParameters;
   	CrystalInst = GetWorld()->SpawnActor<ACrystal>(CrystalToSpawn, CrystalSpawnLocation, SpawnRotation, SpawnParameters);

	if (!CrystalInst)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn crystal!"));
		return;
	}
	
	OnCrystalSpawnedDelegate.Broadcast(CrystalSpawnLocation, CrystalInst);
		
}

void UCrystalSpawnerComp::StartSpawning()
{
	if (AActor* Owner = GetOwner())
	{
		float RandomInterval = FMath::FRandRange(SpawnIntervalMin, SpawnIntervalMax);
		Owner->GetWorldTimerManager().SetTimer(SpawningHandle, this, FTimerDelegate::TMethodPtr<UCrystalSpawnerComp>(&UCrystalSpawnerComp::SpawnCrystal), RandomInterval, true);
	}
}

void UCrystalSpawnerComp::StopSpawning(APawn* Looser)
{
	GetWorld()->GetTimerManager().ClearTimer(SpawningHandle);
}



