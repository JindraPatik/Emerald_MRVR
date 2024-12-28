#include "AI_Component.h"
#include "Engine/TargetPoint.h"
#include "CrystalSpawnerComp.h"
#include "EngineUtils.h"
#include "MilitaryBaseComp.h"
#include "ResourcesComponent.h"
#include "UnitMovementComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/CORE/Gamemode_Single.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"


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
	UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
	float DistanceToCrystal = FVector::Dist(MilitaryBaseComp->GetBaseInstance()->GetActorLocation(), CrystalLocation);
	return DistanceToCrystal;
}

void UAI_Component::SpawnHarvester(UMilitaryBaseComp* MilitaryBaseComp)
{
	APawn* AIPawn = Cast<APawn>(GetOwner());
	if (AIPawn)
	{
		if (MineModule)
		{
			TSubclassOf<AUnit> UnitClassToSpawn = MineModule->UnitToSpawn;
			UWorld* World = GetWorld();

			FVector UnitSpawnLocation = MilitaryBaseComp->MyBaseInstance->SpawnPoint_Ground->GetComponentLocation();
			FRotator UnitSpawnRotation = MilitaryBaseComp->MyBaseInstance->SpawnPoint_Ground->GetComponentRotation();
			FActorSpawnParameters UnitSpawnParams;
			UnitSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			UnitSpawnParams.Owner = GetOwner();
			UUnitDataAsset* SpawnedUnitDataAsset = MineModule->UnitToSpawnData;
			AUnit* UnitInstance = World->SpawnActor<AUnit>(UnitClassToSpawn, UnitSpawnLocation, UnitSpawnRotation, UnitSpawnParams);
			
			UnitInstance->UnitMovementComponent->UnitSpeed = SpawnedUnitDataAsset->Speed;
			UnitInstance->Speed = SpawnedUnitDataAsset->Speed;
			UnitInstance->Price = SpawnedUnitDataAsset->Price;
			UnitInstance->Strenght = SpawnedUnitDataAsset->Strength;
			UnitInstance->Damage = SpawnedUnitDataAsset->Damage;
			UResourcesComponent* ResourcesComponent = AIPawn->FindComponentByClass<UResourcesComponent>();
			if (ResourcesComponent)
			{
				ResourcesComponent->UpdateResources(SpawnedUnitDataAsset->Price);
			}
		}
	}
}

void UAI_Component::OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst)
{
	bool bShouldSendHarvester = GetMyDistanceFromCrystal(CrystalLoc) <= GetDistanceBetweenCrystalSpawners()/DistanceToCrystalTolerance;
	UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
	
	if (MilitaryBaseComp && bShouldSendHarvester && MilitaryBaseComp->HasEnoughResources(MineModule))
	{
		SpawnHarvester(MilitaryBaseComp);
	}
}





