#include "CrystalSpawnerComp.h"

#include "ComponentUtils.h"
#include "EngineUtils.h"
#include "Emerald_MRVR/Crystal.h"
#include "Emerald_MRVR/CORE/EK_GameMode.h"
#include "Engine/TargetPoint.h"


UCrystalSpawnerComp::UCrystalSpawnerComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UCrystalSpawnerComp::BeginPlay()
{
	Super::BeginPlay();
}


void UCrystalSpawnerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCrystalSpawnerComp::SpawnCrystal()
{
	TArray<ATargetPoint*> Targets;
	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		if (It->ActorHasTag("CrystalBoundry"))
		{
			Targets.Add(*It);
		}
	}
	FVector Target1_Pos = Targets[0]->GetActorLocation();
	FVector Target2_Pos = Targets[1]->GetActorLocation();
	
    // Generates num between 0 and 1
    float RandomAlpha = FMath::FRand();
	
    FVector RandomCrystalLocation = FMath::Lerp(Target1_Pos, Target2_Pos, RandomAlpha);
    FRotator SpawnRotation = FRotator(0.f,FMath::RandRange(0.f, 360.f), 0.f);
    FActorSpawnParameters SpawnParameters;
   	GetWorld()->SpawnActor<ACrystal>(CrystalToSpawn, RandomCrystalLocation, SpawnRotation, SpawnParameters);	
		
}

void UCrystalSpawnerComp::StartSpawning()
{
	if (AActor* Owner = GetOwner())
	{
		float RandomInterval = FMath::FRandRange(SpawnIntervalMin, SpawnIntervalMax);
		Owner->GetWorldTimerManager().SetTimer(SpawningHandle, this, FTimerDelegate::TMethodPtr<UCrystalSpawnerComp>(&UCrystalSpawnerComp::SpawnCrystal), RandomInterval, true);
	}
}

