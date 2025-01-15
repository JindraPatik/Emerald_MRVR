#include "AI_Component.h"
#include "Engine/TargetPoint.h"
#include "CrystalSpawnerComp.h"
#include "EngineUtils.h"
#include "HarvestComponent.h"
#include "MilitaryBaseComp.h"
#include "ResourcesComponent.h"
#include "UnitMovementComponent.h"
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

	AI_Pawn = Cast<APawn>(GetOwner());
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
		 					// MilitaryBaseComp->OnUnitSpawnedDelegate.AddDynamic(this, &UAI_Component::OnUnitOccured);
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

void UAI_Component::GetAvailableAttackingUnits()
{
	UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
	if (MilitaryBaseComp)
	{
		for (AModuleActor* AvailableUnit : MilitaryBaseComp->AvailableModulesActors)
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

AUnit* UAI_Component::SpawnUnit(AModuleActor* Module)
{
	UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
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

void UAI_Component::ChooseOptimalUnit(AUnit* AttackerUnit, UMilitaryBaseComp* MilitaryBaseComp, TArray<AModuleActor*> Availables)
{
	for (AModuleActor* ReactUnit : Availables)
	{
		if (AttackerUnit->Strenght < ReactUnit->BuildingDataAsset->UnitToSpawnData->Strength) // Has stronger
		{
			TArray<AModuleActor*> AvailableStrongerUnits; // All available stronger Units
			AvailableStrongerUnits.Add(ReactUnit);

			for (AModuleActor* AvailableStronger : AvailableStrongerUnits)
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
				TArray<AModuleActor*> AvailableStrongerUnits; // All available stronger Units
				AvailableStrongerUnits.Add(ReactUnit);


				for (AModuleActor* AvailableStronger : AvailableStrongerUnits)
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

void UAI_Component::EnableSpawning()
{
	bSpawningEnabled = true;
}

void UAI_Component::Cooldown(float CD_Time)
{
	GetWorld()->GetTimerManager().SetTimer(CD_Handle, this, &UAI_Component::EnableSpawning, CD_Time, false);
}

void UAI_Component::OnUnitOccured(AUnit* Unit, AActor* Owner)
{
	GetWorld()->GetTimerManager().ClearTimer(RandomSpawn_Handle); // Clears random spawn timer
	if (Unit->FindComponentByClass<UHarvestComponent>()) return; // If Harvester, don't react;
	UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
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

void UAI_Component::SpawnRandomUnit()
{
	if (FightStatus == EUnitFightStatus::EIsAttacking)
	{
		UMilitaryBaseComp* MilitaryBaseComp = GetOwner()->FindComponentByClass<UMilitaryBaseComp>();
		if (MilitaryBaseComp)
		{
			if (MilitaryBaseComp->AvailableModulesActors.Num() > 0)
			{
				int32 RandomIndex = FMath::RandRange(0, MilitaryBaseComp->AvailableModulesActors.Num() - 1);
				AModuleActor* RandomItem = MilitaryBaseComp->AvailableModulesActors[RandomIndex];
				
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

void UAI_Component::HandleRandomSpawn()
{
	float RandomSpawnInterval = FMath::RandRange(RandomSpawnMin, RandomSpawnMax);
	GetWorld()->GetTimerManager().SetTimer(RandomSpawn_Handle, this, &UAI_Component::SpawnRandomUnit, RandomSpawnInterval);
}


void UAI_Component::TryToDefend(UMilitaryBaseComp* MilitaryBaseComp, TArray<AModuleActor*> Availables)
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


















