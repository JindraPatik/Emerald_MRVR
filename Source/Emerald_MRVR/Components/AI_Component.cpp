#include "AI_Component.h"
#include "Engine/TargetPoint.h"
#include "CrystalSpawnerComp.h"
#include "DownScaleComponent.h"
#include "EngineUtils.h"
#include "MilitaryBaseComp.h"
#include "ResourcesComponent.h"
#include "UnitMovementComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/CORE/Gamemode_Single.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"

UAI_Component::UAI_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAI_Component::BeginPlay()
{
	Super::BeginPlay();

	GetAvailableUnits();

	
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

		 	for (TActorIterator<AMR_General> It(GetWorld()); It; ++It)
		 	{
		 		AMR_General* General = Cast<AMR_General>(*It);
			    if (General)
			    {
					UMilitaryBaseComp* MilitaryBaseComp = General->FindComponentByClass<UMilitaryBaseComp>();
						if (MilitaryBaseComp)
						{
		 					MilitaryBaseComp->OnUnitSpawnedDelegate.AddDynamic(this, &UAI_Component::OnUnitOccured);
						}
			    }
		 	}
		 }
	}
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

void UAI_Component::GetAvailableUnits()
{
	UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
	if (MilitaryBaseComp)
	{
		for (UBuildingDataAsset* AvailableUnit : MilitaryBaseComp->AvailableModules)
		{
			if (AvailableUnit)
			{
				if (!AvailableUnit->UnitToSpawnData->IsFlyingUnit)
				{
					AvailableGroundUnits.Add(AvailableUnit->UnitToSpawnData);
				}
				else
				{
					AvailableFlyingUnits.Add(AvailableUnit->UnitToSpawnData);
				}
			}
		}
	}
}

void UAI_Component::SpawnUnit(UUnitDataAsset* UnitData, bool bIsFlying)
{
	UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnPointLoc;
	FRotator SpawnPointRot;
	bIsFlying ? SpawnPointLoc = MilitaryBaseComp->MyBaseInstance->SpawnPoint_Air->GetComponentLocation() : SpawnPointLoc = MilitaryBaseComp->MyBaseInstance->SpawnPoint_Ground->GetComponentLocation();
	bIsFlying ? SpawnPointRot = MilitaryBaseComp->MyBaseInstance->SpawnPoint_Air->GetComponentRotation() : SpawnPointRot = MilitaryBaseComp->MyBaseInstance->SpawnPoint_Ground->GetComponentRotation();

	AUnit* ReactUnit = GetWorld()->SpawnActor<AUnit>(SpawnPointLoc, SpawnPointRot, SpawnParameters);
	if (ReactUnit)
	{
		ReactUnit->Body->SetStaticMesh(UnitData->SM_Unit);
		ReactUnit->DownScaleComponent->DownscaleFactor = 0.01f;
		ReactUnit->UnitMovementComponent->UnitSpeed = UnitData->Speed;
		ReactUnit->Speed = UnitData->Speed;
		ReactUnit->Price = UnitData->Price;
		ReactUnit->Strenght = UnitData->Strength;
		ReactUnit->Damage = UnitData->Damage;
		ReactUnit->bIsFlyingUnit = UnitData->IsFlyingUnit;
	}
}

void UAI_Component::OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst)
{
	bool bShouldSendHarvester = GetMyDistanceFromCrystal(CrystalLoc) <= GetDistanceBetweenCrystalSpawners()/DistanceToCrystalTolerance;
	UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
	
	if (MilitaryBaseComp && bShouldSendHarvester && MilitaryBaseComp->HasEnoughResources(MineModule))
	{
		FTimerHandle HarvesterSpawnDelayHandle;
		float SimulatedDelay = FMath::FRandRange(0.f, MaxSimulatedDelayToSpawnHarvester);
		GetWorld()->GetTimerManager().SetTimer
			(HarvesterSpawnDelayHandle,[this, MilitaryBaseComp]()
			{ this->SpawnHarvester(MilitaryBaseComp); },
			SimulatedDelay,
			false
			);
	}
}

// TODO
void UAI_Component::OnUnitOccured(AUnit* Unit, AActor* Owner)
{
	DBG(4.f, "Unit occured!, Unit occured! Unit occured! Unit occured!")

	UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
	
	if (Owner && Owner != GetOwner() && MilitaryBaseComp) // If it's not my player
	{
		if (Unit && !Unit->bIsFlyingUnit) // React ground Units
		{
			for (UUnitDataAsset* ReactUnit : AvailableGroundUnits)
			{
				if (Unit->Strenght < ReactUnit->Strength) // Has stronger
				{
					// Has Enough res??
					SpawnUnit(ReactUnit, ReactUnit->IsFlyingUnit);
					return;
				}
				else if (Unit->Strenght == ReactUnit->Strength) // Has same 
				{
					SpawnUnit(ReactUnit, ReactUnit->IsFlyingUnit);
					return;
				}
				else // Doesn't have propriate ReactUnit 
				{
					DBG(3.f, "Don't have propiate ground unit");
				}
			}
		}
		else // React flying Units
		{
			
		}
	}
}









