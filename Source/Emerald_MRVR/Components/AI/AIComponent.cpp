#include "AIComponent.h"
#include "EngineUtils.h"
#include "Emerald_MRVR/Components/MilitaryStation/MilitaryStationComp.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Emerald_MRVR/Actors/Units/SpecialUnits/Digger.h"
#include "Emerald_MRVR/Components/Resources/CrystalSpawnerComp.h"
#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/CORE/GameModes/GameModeSingle.h"
#include "Emerald_MRVR/CORE/Pawns/VRPawn.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
#include "Engine/TargetPoint.h"

UAIComponent::UAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAIComponent::BeginPlay()
{
	Super::BeginPlay();

	AI_Pawn = Cast<APawn>(GetOwner());
	AEK_GameStateInst = Cast<AEKGameState>(GetWorld()->GetGameState());
	
	if (AEK_GameStateInst)
	{
		AGameModeSingle* Gamemode_Single = Cast<AGameModeSingle>(AEK_GameStateInst->AuthorityGameMode);
		 if (Gamemode_Single)
		 {
			 UCrystalSpawnerComp* CrystalSpawnerComp = Cast<UCrystalSpawnerComp>(Gamemode_Single->FindComponentByClass<UCrystalSpawnerComp>());
			 if (CrystalSpawnerComp)
			 {
				 CrystalSpawnerComp->OnCrystalSpawnedDelegate.AddDynamic(this, &UAIComponent::OnCrystalOccured);
			 }

		 	for (TActorIterator<AVRPawn> It(GetWorld()); It; ++It)
		 	{
		 		AVRPawn* VR_Pawn = Cast<AVRPawn>(*It);
			    if (VR_Pawn)
			    {
					UMilitaryStationComp* MilitaryStationComp = VR_Pawn->FindComponentByClass<UMilitaryStationComp>();
						if (MilitaryStationComp)
						{
		 					MilitaryStationComp->OnUnitSpawnedDelegate.AddDynamic(this, &UAIComponent::OnUnitOccured);
						}
			    }
		 	}
		 }
	}
}

float UAIComponent::GetDistanceBetweenCrystalSpawners() const
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

float UAIComponent::GetMyDistanceFromCrystal(FVector CrystalLocation) const
{
	UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
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
	APawn* AIPawn = Cast<APawn>(GetOwner());
	if (AIPawn)
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
	
}

void UAIComponent::GetAvailableAttackingUnits()
{
	UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
	if (MilitaryBaseComp)
		bIsReversed = MilitaryBaseComp->bIsReversed;
		{
			for (ABuilding* AvailableUnit : MilitaryBaseComp->AvailableBuildingsActors)
			{
				if (AvailableUnit)
				{
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
		}
}

AUnit* UAIComponent::SpawnUnit(ABuilding* Module)
{
	UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
	if (MilitaryBaseComp->HasEnoughResources(Module->BuildingDataAsset) && bSpawningEnabled)
	{
		if (AI_Pawn)
		{
			AUnit* Unit = MilitaryBaseComp->SpawnUnit(AI_Pawn, Module);
			return Unit;
		}
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
				AUnit* UnitInstance = MilitaryBaseComp->SpawnUnit(AI_Pawn, ReactUnit);
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

void UAIComponent::Cooldown(float CD_Time)
{
	GetWorld()->GetTimerManager().SetTimer(CD_Handle, this, &UAIComponent::EnableSpawning, CD_Time, false);
}

void UAIComponent::OnUnitOccured(AUnit* Unit, AActor* Owner)
{
	GetWorld()->GetTimerManager().ClearTimer(RandomSpawn_Handle); // Clears random spawn timer
	
	ADigger* Digger = Cast<ADigger>(Unit);
	if (!Digger)
	{
		return;
	}
	UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
	CheapestStronger = nullptr;
	CheapestSame = nullptr;
	float ProbabilityToSpawn = FMath::FRandRange(0.f, 100.f);
	
	if (Owner && Owner != GetOwner() && MilitaryBaseComp && ProbabilityToSpawn <= ProbabilityFactorToSpawnReactUnit) // If it's not my player
	{
		if (Unit && !Unit->bIsFlyingUnit) // React ground Units
		{
			ChooseOptimalUnit(Unit, MilitaryBaseComp, AvailableGroundUnits);
		}
		
		else // React to flying Units
		{
			ChooseOptimalUnit(Unit, MilitaryBaseComp, AvailableFlyingUnits);
		}
	}
}

void UAIComponent::SpawnRandomUnit()
{
	if (FightStatus == EUnitFightStatus::EIsAttacking)
	{
		UMilitaryStationComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryStationComp>();
		if (MilitaryBaseComp)
		{
			if (MilitaryBaseComp->AvailableBuildingsActors.Num() > 0)
			{
				int32 RandomIndex = FMath::RandRange(0, MilitaryBaseComp->AvailableBuildingsActors.Num() - 1);
				ABuilding* RandomItem = MilitaryBaseComp->AvailableBuildingsActors[RandomIndex];
				
				SpawnUnit(RandomItem);
				FightStatus = EIsAttacking;
				HandleRandomSpawn();
			}
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