#include "MilitaryBaseComp.h"

#include "EngineUtils.h"
#include "ResourcesComponent.h"
#include "UnitMovementComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/ModuleActor.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/EK_GameMode.h"
#include "Emerald_MRVR/CORE/Gamemode_Single.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/CORE/UnitAIController.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
#include "Engine/TargetPoint.h"
#include "Net/UnrealNetwork.h"


UMilitaryBaseComp::UMilitaryBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	SpawnPointForMilitaryBase = CreateDefaultSubobject<ATargetPoint>("MilitaryBaseTargetPoint");
	SetIsReplicatedByDefault(true);
}

void UMilitaryBaseComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMilitaryBaseComp, AvailableModules);
	DOREPLIFETIME(UMilitaryBaseComp, SpawnPointForMilitaryBase);
	//DOREPLIFETIME(UMilitaryBaseComp, SpawnLocation);
	//DOREPLIFETIME(UMilitaryBaseComp, SpawnRotation);
	DOREPLIFETIME(UMilitaryBaseComp, MyBaseInstance);
	DOREPLIFETIME(UMilitaryBaseComp, UnitSpawnLocation);
	DOREPLIFETIME(UMilitaryBaseComp, UnitSpawnRotation);
}

void UMilitaryBaseComp::BeginPlay()
{
	Super::BeginPlay();
	
	MyOwner = Cast<APawn>(GetOwner());
	MultiGameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	ResourcesComponentInst = Cast<UResourcesComponent>(GetOwner()->FindComponentByClass<UResourcesComponent>());
	// GetMilitaryBaseSpawnPoint();
}

void UMilitaryBaseComp::SetSpawnPointForBase()
{
	if (MultiGameMode && !GetOwner()->HasAuthority())
	{
		Server_SetSpawnPointForBase();
		return;
	}
	
	TArray<ATargetPoint*> AllBaseTargetPoints;
	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		ATargetPoint* BaseTargetPoint = *It;
		if (BaseTargetPoint && BaseTargetPoint->ActorHasTag("BaseSpawnPoint"))
		{
			AllBaseTargetPoints.Add(BaseTargetPoint);
		}
	}
	if (AllBaseTargetPoints.Num() > 0)
	{
		SpawnPointForMilitaryBase = AllBaseTargetPoints[0];
		AllBaseTargetPoints[0]->Destroy();
	}
}

void UMilitaryBaseComp::Server_SetSpawnPointForBase_Implementation()
{
	SetSpawnPointForBase();
}


void UMilitaryBaseComp::SpawnMilitaryBase(APawn* OwningPawn)
{
	if (MultiGameMode && !OwningPawn->HasAuthority())
	{
		Server_SpawnMilitaryBase(OwningPawn);
		return;
	}
	
	SetSpawnPointForBase();
	if (SpawnPointForMilitaryBase)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = OwningPawn;
		SpawnParameters.Owner = OwningPawn;
		FVector SpawnLocation = SpawnPointForMilitaryBase->GetActorLocation();
		FRotator SpawnRotation = SpawnPointForMilitaryBase->GetActorRotation();
		
		if (OwningPawn)
		{
			MyBaseInstance = GetWorld()->SpawnActor<AMilitaryBase>(MilitaryBase, SpawnLocation, SpawnRotation, SpawnParameters);
			// ResourcesComponentInst->StartGrowResources();
		}
	}	
}

void UMilitaryBaseComp::Server_SpawnMilitaryBase_Implementation(APawn* InOwner)
{
	SpawnMilitaryBase(InOwner);
}


void UMilitaryBaseComp::SpawnModules(APawn* OwningPawn)
{
	if (MultiGameMode && !GetOwner()->HasAuthority())
	{
		Server_SpawnModule(OwningPawn);
		return;
	}

	if (SpawnPointForMilitaryBase)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = OwningPawn;
		SpawnParameters.Owner = OwningPawn;
		
		if (AvailableModules.Num() > 0)
		{
			for (UBuildingDataAsset* Module : AvailableModules)
			{
				if (MyBaseInstance && MyBaseInstance->ModulePositions.Num() > 0)
				{
					for (USceneComponent* ModulePos : MyBaseInstance->ModulePositions)
					{
						if (ModulePos->ComponentHasTag(Module->BuildingName))
						{
							FVector ModuleSpawnLoc = ModulePos->GetComponentLocation();
							FRotator ModuleSpawnRot = ModulePos->GetComponentRotation();
							
							AModuleActor* ModuleInstance = GetWorld()->SpawnActor<AModuleActor>(Module->ModuleClass, ModuleSpawnLoc, ModuleSpawnRot, SpawnParameters);
							if (ModuleInstance)
							{
								ModuleInstance->BuildingDataAsset = Module;
							}
							if (MultiGameMode)
							{
								ModuleInstance->SetReplicates(true);
							}
						}
					}
				}
			}
		}
	}
}

void UMilitaryBaseComp::Server_SpawnModule_Implementation(APawn* OwningPawn)
{
	SpawnModules(OwningPawn);
}

/*void UMilitaryBaseComp::GetMilitaryBaseSpawnPoint()
{
	AGamemode_Single* Gamemode_Single = Cast<AGamemode_Single>(GetWorld()->GetAuthGameMode());

	if (MultiGameMode && !General->HasAuthority())
	{
		Server_GetMilitaryBaseSpawnPoint();
		return;
	}
	
	if (MultiGameMode && MultiGameMode->TargetPoints.Num() > 0)
	{
		SpawnPointForMilitaryBase = MultiGameMode->TargetPoints.IsValidIndex(0) ? MultiGameMode->TargetPoints[0] : nullptr;
		SpawnPoint = MultiGameMode->TargetPoints[0]->GetTransform();
		MultiGameMode->TargetPoints.RemoveAt(0);
		SpawnLocation = SpawnPointForMilitaryBase->GetActorLocation();
		SpawnRotation = SpawnPointForMilitaryBase->GetActorRotation();
		return;
	}
	
	if (Gamemode_Single && Gamemode_Single->TargetPoints.Num() > 0)
	{
		SpawnPointForMilitaryBase = Gamemode_Single->TargetPoints.IsValidIndex(0) ? Gamemode_Single->TargetPoints[0] : nullptr;
		SpawnPoint = Gamemode_Single->TargetPoints[0]->GetTransform();
		Gamemode_Single->TargetPoints.RemoveAt(0);
		SpawnLocation = SpawnPointForMilitaryBase->GetActorLocation();
		SpawnRotation = SpawnPointForMilitaryBase->GetActorRotation();
		return;
	}
}*/

/*void UMilitaryBaseComp::Server_GetMilitaryBaseSpawnPoint_Implementation()
{
	GetMilitaryBaseSpawnPoint();
}*/

void UMilitaryBaseComp::SpawnUnit(APawn* InstigatorPawn, AModuleActor* Module)
{
	if (MultiGameMode && GetOwner()->HasAuthority())
	{
		Server_SpawnUnit(InstigatorPawn, Module);
		return;
	}
	
	if (InstigatorPawn && MyBaseInstance && Module)
	{
		UBuildingDataAsset* BuildingDataAsset = Module->BuildingDataAsset;
		if (BuildingDataAsset)
		{
			TSubclassOf<AUnit> UnitClassToSpawn = BuildingDataAsset->UnitToSpawn;
			TObjectPtr<UWorld> World = GetWorld();

			if (!BuildingDataAsset->UnitToSpawnData->IsFlyingUnit)
			{
				UnitSpawnLocation = MyBaseInstance->SpawnPoint_Ground->GetComponentLocation();
				UnitSpawnRotation = MyBaseInstance->SpawnPoint_Ground->GetComponentRotation();
			}
			else
			{
				UnitSpawnLocation = MyBaseInstance->SpawnPoint_Air->GetComponentLocation();
				UnitSpawnRotation = MyBaseInstance->SpawnPoint_Air->GetComponentRotation();
			}
			FActorSpawnParameters UnitSpawnParams;
			UnitSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			UnitSpawnParams.Owner = InstigatorPawn;
			UnitSpawnParams.Instigator = InstigatorPawn;

			if (HasEnoughResources(BuildingDataAsset))
			{
				UUnitDataAsset* SpawnedUnitDataAsset = BuildingDataAsset->UnitToSpawnData;
				ResourcesComponentInst->UpdateResources(SpawnedUnitDataAsset->Price);
				AUnit* UnitInstance = World->SpawnActor<AUnit>(UnitClassToSpawn, UnitSpawnLocation, UnitSpawnRotation, UnitSpawnParams);
				UnitInstance->UnitMovementComponent->UnitSpeed = SpawnedUnitDataAsset->Speed;
				UnitInstance->Speed = SpawnedUnitDataAsset->Speed;
				UnitInstance->Price = SpawnedUnitDataAsset->Price;
				UnitInstance->Strenght = SpawnedUnitDataAsset->Strength;
				UnitInstance->Damage = SpawnedUnitDataAsset->Damage;
			}
			else
			{
				DBG(3, "NotEnoughRsources")
			}
			return;
		}
	}
	DBG(3, "MBC: NOT INstigator || BaseInstance || Module")
}


void UMilitaryBaseComp::Server_SpawnUnit_Implementation(APawn* InstigatorPawn, AModuleActor* Module)
{
	SpawnUnit(InstigatorPawn, Module);
}


bool UMilitaryBaseComp::HasEnoughResources(UBuildingDataAsset* BuildingDataAsset) const
{
		if (MultiGameMode && !GetOwner()->HasAuthority())
		{
			Server_HasEnoughResources(BuildingDataAsset);
		}
	
		if (ResourcesComponentInst)
		{
			if (ResourcesComponentInst->AvailableResources >= BuildingDataAsset->UnitToSpawnData->Price)
			{
				return true;
			}
			return false;
		}
	return false;
}

void UMilitaryBaseComp::Server_HasEnoughResources_Implementation(UBuildingDataAsset* BuildingDataAsset) const
{
	HasEnoughResources(BuildingDataAsset);
}
