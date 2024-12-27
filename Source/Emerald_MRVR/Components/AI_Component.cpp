#include "AI_Component.h"
#include "Engine/TargetPoint.h"
#include "CrystalSpawnerComp.h"
#include "EngineUtils.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/CORE/Gamemode_Single.h"


UAI_Component::UAI_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAI_Component::BeginPlay()
{
	Super::BeginPlay();
	AEK_GameStateInst = Cast<AEKGameState>(GetWorld()->GetGameState());
	if (AEK_GameStateInst)
	{
		 AGamemode_Single* Gamemode_Single = Cast<AGamemode_Single>(AEK_GameStateInst->AuthorityGameMode);
		 if (Gamemode_Single)
		 {
			 UCrystalSpawnerComp* CrystalSpawnerComp = Cast<UCrystalSpawnerComp>(Gamemode_Single->FindComponentByClass<UCrystalSpawnerComp>());
			 if (CrystalSpawnerComp)
			 {
				 CrystalSpawnerComp->OnCrystalSpawnedDelegate.AddDynamic(this, &UAI_Component::OnCrystalOccured);
			 }
		 }
	}
}

void UAI_Component::PostInitProperties()
{
	Super::PostInitProperties();
}

void UAI_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UAI_Component::GetDistanceBetweenCrystalSpawners() const
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

	float DistanceBetweenCrystals = FVector::Dist(Target1_Pos, Target2_Pos);

	return DistanceBetweenCrystals;
}

float UAI_Component::GetMyDistanceFromCrystal(FVector CrystalLocation) const
{
	float DistanceToCrystal = FVector::Dist(GetOwner()->GetActorLocation(), CrystalLocation);
	return DistanceToCrystal;
}

void UAI_Component::OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst)
{
	bool bShouldSendHarvester = GetMyDistanceFromCrystal(CrystalLoc) <= GetDistanceBetweenCrystalSpawners()/1.5; 
	
	if (bShouldSendHarvester)
	{
		
	}
}





