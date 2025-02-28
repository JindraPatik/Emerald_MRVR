#include "AIComponent.h"
#include "EngineUtils.h"
#include "Emerald_MRVR/Actors/Support/SpawnPointCrystal.h"
#include "Emerald_MRVR/Components/MilitaryStation/MilitaryStationComp.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Emerald_MRVR/Actors/Units/SpecialUnits/Digger.h"
#include "Emerald_MRVR/Components/Resources/CrystalSpawnerComp.h"
#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/CORE/GameModes/GameModeSingle.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"

UAIComponent::UAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAIComponent::BeginPlay()
{
	Super::BeginPlay();

	AIPawn = Cast<APawn>(GetOwner());
	if (!AIPawn)
	{
		return;
	}
	AEK_GameStateInst = Cast<AEKGameState>(GetWorld()->GetGameState());
	
	if (!AEK_GameStateInst)
	{
		return;
	}
	AGameModeSingle* Gamemode_Single = Cast<AGameModeSingle>(AEK_GameStateInst->AuthorityGameMode);
	if (!Gamemode_Single)
	{
		return;
	}
	UCrystalSpawnerComp* CrystalSpawnerComp = Cast<UCrystalSpawnerComp>(Gamemode_Single->FindComponentByClass<UCrystalSpawnerComp>());
	if (CrystalSpawnerComp)
	{
		CrystalSpawnerComp->OnCrystalSpawnedDelegate.AddDynamic(this, &UAIComponent::OnCrystalOccured);
	}

	for (TActorIterator<AVRPawn> It(GetWorld()); It; ++It)
	{
		AVRPawn* VRPawn = Cast<AVRPawn>(*It);
		UMilitaryStationComp* MilitaryStationComp = VRPawn->FindComponentByClass<UMilitaryStationComp>();
	    if (!VRPawn || !MilitaryStationComp)
	    {
		    return;
	    }

		MilitaryStationComp->OnUnitSpawnedDelegate.AddDynamic(this, &UAIComponent::OnUnitOccured);
	}
}

float UAIComponent::GetDistanceBetweenCrystalSpawners() const
{
	TArray<ASpawnPointCrystal*> SpawnPoints;
	for (TActorIterator<ASpawnPointCrystal> It(GetWorld()); It; ++It)
	{
		SpawnPoints.Add(*It);
	}
	FVector Point1_Pos = SpawnPoints[0]->GetActorLocation();
	FVector Point2_Pos = SpawnPoints[1]->GetActorLocation();

	float DistanceBetweenCrystals = FVector::Dist(Point1_Pos, Point2_Pos);

	return DistanceBetweenCrystals;
}

float UAIComponent::GetMyDistanceFromCrystal(FVector CrystalLocation) const
{
	UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
	if (!MilitaryBaseComp)
	{
		return 0;
	}
	
	float DistanceToCrystal = FVector::Dist(MilitaryBaseComp->GetMilitaryStationInstance()->GetActorLocation(), CrystalLocation);
	return DistanceToCrystal;
}

void UAIComponent::OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst)
{
	bool bShouldSendDigger = GetMyDistanceFromCrystal(CrystalLoc) <= GetDistanceBetweenCrystalSpawners()/DistanceToCrystalTolerance;
	UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
	
	if (MilitaryBaseComp && bShouldSendDigger && MilitaryBaseComp->HasEnoughResources(MineBuilding))
	{
		FTimerHandle DiggerSpawnDelayHandle;
		float SimulatedDelay = FMath::FRandRange(0.f, MaxSimulatedDelayToSpawnDigger);
		GetWorld()->GetTimerManager().SetTimer
			(DiggerSpawnDelayHandle,[this, MilitaryBaseComp]()
			{ this->SpawnDigger(MilitaryBaseComp); },
			SimulatedDelay,
			false
			);
	}
}

void UAIComponent::SpawnDigger(UMilitaryStationComp* MilitaryBaseComp)
{
	for (ABuilding* Module : MilitaryBaseComp->AvailableBuildingsActors)
	{
		if (Module->BuildingDataAsset == MineBuilding)
		{
			SpawnUnit(Module);
			break;
		}
	}
}

void UAIComponent::GetAvailableAttackingUnits()
{
	UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
	if (!MilitaryBaseComp)
	{
		return;
	}
	bIsReversed = MilitaryBaseComp->bIsReversed;
	
	for (ABuilding* AvailableUnit : MilitaryBaseComp->AvailableBuildingsActors)
	{
		if (!AvailableUnit)
		{
			return;
		}
		if (!AvailableUnit->BuildingDataAsset->UnitToSpawnData->IsFlyingUnit)
		{
			if (AvailableUnit->BuildingDataAsset->UnitToSpawnData->IsAttacker)
			{
				AvailableGroundUnits.Add(AvailableUnit);
			}
		}
		else
		{
			if (AvailableUnit->BuildingDataAsset->UnitToSpawnData->IsAttacker)
			{
				AvailableFlyingUnits.Add(AvailableUnit);
			}
		}
	}
}

AUnit* UAIComponent::SpawnUnit(ABuilding* Module)
{
	UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
	if (!MilitaryBaseComp)
	{
		return nullptr;
	}
	
	if (MilitaryBaseComp->HasEnoughResources(Module->BuildingDataAsset) && bSpawningEnabled)
	{
		if (!AIPawn)
		{
			return nullptr;
		}
		AUnit* Unit = MilitaryBaseComp->SpawnUnit(AIPawn, Module);
		return Unit;
	}
	return nullptr;
}

void UAIComponent::ChooseOptimalUnit(AUnit* AttackerUnit, UMilitaryStationComp* MilitaryBaseComp, TArray<ABuilding*> Availables)
{
	for (ABuilding* ReactUnit : Availables)
	{
		if (AttackerUnit->Strenght < ReactUnit->BuildingDataAsset->UnitToSpawnData->Strength) // Has stronger
		{
			TArray<ABuilding*> AvailableStrongerUnits; // All available stronger Units
			AvailableStrongerUnits.Add(ReactUnit);

			for (ABuilding* AvailableStronger : AvailableStrongerUnits)
			{
				if (CheapestStronger == nullptr)
				{
					CheapestStronger = AvailableStronger; // If reference not valid add one
				}

				if (CheapestStronger &&AvailableStronger->BuildingDataAsset->UnitToSpawnData->Price < CheapestStronger->BuildingDataAsset->UnitToSpawnData->Price) // If there is other unit which is stronger add to temp ref
				{
					CheapestStronger = AvailableStronger; 
				}
			}
					
			if (MilitaryBaseComp->HasEnoughResources(CheapestStronger->BuildingDataAsset)) // Has enough res. to spawn? Do it!
			{
				FightStatus = EIsDefending;
				AUnit* UnitInstance = MilitaryBaseComp->SpawnUnit(AIPawn, ReactUnit);
				if (UnitInstance && UndefendedUnit && (UnitInstance->Strenght == UndefendedUnit->Strenght || UnitInstance->Strenght > UndefendedUnit->Strenght) && (UnitInstance->bIsFlyingUnit == UndefendedUnit->bIsFlyingUnit))
				{
					UndefendedUnit = nullptr;
					FightStatus = EIsAttacking;
				}
				return;
			}
		}
				
		else if (AttackerUnit->Strenght == ReactUnit->BuildingDataAsset->UnitToSpawnData->Strength) // Has same 
		{
			{
				TArray<ABuilding*> AvailableStrongerUnits; // All available stronger Units
				AvailableStrongerUnits.Add(ReactUnit);


				for (ABuilding* AvailableStronger : AvailableStrongerUnits)
				{
					if (CheapestSame == nullptr)
					{
						CheapestSame = AvailableStronger; // If reference not valid add one
					}
					else
					{
						if (AvailableStronger->BuildingDataAsset->UnitToSpawnData->Price < CheapestSame->BuildingDataAsset->UnitToSpawnData->Price) // If there is other unit which is stronger add to temp ref
						{
							CheapestSame = AvailableStronger; 
						}
					}
				}
					
				if (MilitaryBaseComp->HasEnoughResources(CheapestSame->BuildingDataAsset)) // Has enough res. to spawn? Do it!
				{
					FightStatus = EIsDefending;
					AUnit* UnitInstance = SpawnUnit(ReactUnit);
					if (UnitInstance && UndefendedUnit && UnitInstance->Strenght == UndefendedUnit->Strenght && UnitInstance->bIsFlyingUnit == UndefendedUnit->bIsFlyingUnit)
					{
						UndefendedUnit = nullptr;
						FightStatus = EIsAttacking;
					}
					return;
				}
			}
		}
		else // Doesn't have propriate ReactUnit 
		{
			FightStatus = EIsDefending;
			UndefendedUnit = AttackerUnit;

			Defending_Delegate.BindUFunction(this, "TryToDefend", MilitaryBaseComp, Availables);
			GetWorld()->GetTimerManager().SetTimer(Defending_Handle, Defending_Delegate, DefendingRate, false);
		}
	}
}

void UAIComponent::EnableSpawning()
{
	bSpawningEnabled = true;
}

void UAIComponent::Cooldown(float CoolDownTime)
{
	GetWorld()->GetTimerManager().SetTimer(CD_Handle, this, &UAIComponent::EnableSpawning, CoolDownTime, false);
}

void UAIComponent::OnUnitOccured(AUnit* InUnit, AActor* InOwner)
{
	GetWorld()->GetTimerManager().ClearTimer(RandomSpawn_Handle); // Clears random spawn timer
	
	ADigger* Digger = Cast<ADigger>(InUnit);
	if (!Digger)
	{
		return;
	}
	UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
	if (!MilitaryBaseComp)
	{
		return;
	}
	
	CheapestStronger = nullptr;
	CheapestSame = nullptr;
	float ProbabilityToSpawn = FMath::FRandRange(0.f, 100.f);
	
	if (InOwner && InOwner != GetOwner() && MilitaryBaseComp && ProbabilityToSpawn <= ProbabilityFactorToSpawnReactUnit) // If it's not my player
	{
		if (InUnit && !InUnit->bIsFlyingUnit) // React ground Units
		{
			ChooseOptimalUnit(InUnit, MilitaryBaseComp, AvailableGroundUnits);
		}
		
		else // React to flying Units
		{
			ChooseOptimalUnit(InUnit, MilitaryBaseComp, AvailableFlyingUnits);
		}
	}
}

void UAIComponent::SpawnRandomUnit()
{
	if (FightStatus == EUnitFightStatus::EIsAttacking)
	{
		UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
		if (!MilitaryBaseComp)
		{
			return;
		}
		
		if (MilitaryBaseComp->AvailableBuildingsActors.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, MilitaryBaseComp->AvailableBuildingsActors.Num() - 1);
			ABuilding* RandomItem = MilitaryBaseComp->AvailableBuildingsActors[RandomIndex];
			
			SpawnUnit(RandomItem);
			FightStatus = EIsAttacking;
			HandleRandomSpawn();
		}
	}
	
	if (FightStatus == EUnitFightStatus::EIsDefending)
	{
		GetWorld()->GetTimerManager().ClearTimer(RandomSpawn_Handle);
	}
}

void UAIComponent::HandleRandomSpawn()
{
	float RandomSpawnInterval = FMath::RandRange(RandomSpawnMin, RandomSpawnMax);
	GetWorld()->GetTimerManager().SetTimer(RandomSpawn_Handle, this, &UAIComponent::SpawnRandomUnit, RandomSpawnInterval);
}


void UAIComponent::TryToDefend(UMilitaryStationComp* MilitaryBaseComp, TArray<ABuilding*> Availables)
{
	if (UndefendedUnit)
	{
		FightStatus = EIsDefending;
		ChooseOptimalUnit(UndefendedUnit, MilitaryBaseComp, Availables);
	}
	
	if (!UndefendedUnit)
	{
		FightStatus = EIsAttacking;
		SpawnRandomUnit();
		GetWorld()->GetTimerManager().ClearTimer(Defending_Handle);
		Defending_Delegate.Unbind();
		return;
	}
}