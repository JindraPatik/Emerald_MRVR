#include "MilitaryBaseComp.h"
#include "ResourcesComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/ModuleActor.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/EK_GameMode.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
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
	DOREPLIFETIME(UMilitaryBaseComp, SpawnPointForMilitaryBase);
	DOREPLIFETIME(UMilitaryBaseComp, SpawnLocation);
	DOREPLIFETIME(UMilitaryBaseComp, SpawnRotation);
}

void UMilitaryBaseComp::BeginPlay()
{
	Super::BeginPlay();
	General = Cast<AMR_General>(GetOwner());


	if (General->IsLocallyControlled())
	{
		GetMilitaryBaseSpawnPoint();
		SpawnMilitaryBase(General);
		SpawnModules(General);
	}

}

void UMilitaryBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMilitaryBaseComp::SpawnMilitaryBase(AMR_General* OwningPawn)
{
	if (!OwningPawn->HasAuthority())
	{
		Server_SpawnMilitaryBase(OwningPawn);
		return;
	}
		if (SpawnPointForMilitaryBase)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = OwningPawn;
			SpawnParameters.Owner = OwningPawn;

			if (General)
			{
				// Spawn Base Instance
				MyBaseInstance = GetWorld()->SpawnActor<AMilitaryBase>(MilitaryBase, SpawnLocation, SpawnRotation, SpawnParameters);
				General->ResourcesComponent->StartGrowResources();
			}
			
		}
}

void UMilitaryBaseComp::Server_SpawnMilitaryBase_Implementation(AMR_General* OwningPawn)
{
	SpawnMilitaryBase(OwningPawn);
}

void UMilitaryBaseComp::SpawnModules(AMR_General* OwningPawn)
{
	if (!GetOwner()->HasAuthority())
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
				AModuleActor* ModuleInstance = GetWorld()->SpawnActor<AModuleActor>(ModuleClass, SpawnLocation, SpawnRotation, SpawnParameters);
				ModuleInstance->SetReplicates(true);
				if (ModuleInstance)
				{
					ModuleInstance->BuildingDataAsset = Module;
				}
			}
		}
	}
}

void UMilitaryBaseComp::GetMilitaryBaseSpawnPoint()
{
	if (!General->HasAuthority())
	{
		Server_GetMilitaryBaseSpawnPoint();
		return;
	}
	
	AEK_GameMode* GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
		if (!GameMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode is invalid!"));
			return;
		}
	
			if (ensure(GameMode) && (GameMode->TargetPoints.Num() > 0))
			{
				SpawnPointForMilitaryBase = GameMode->TargetPoints.IsValidIndex(0) ? GameMode->TargetPoints[0] : nullptr;
				SpawnPoint = GameMode->TargetPoints[0]->GetTransform();
				GameMode->TargetPoints.RemoveAt(0);
				SpawnLocation = SpawnPointForMilitaryBase->GetActorLocation();
				SpawnRotation = SpawnPointForMilitaryBase->GetActorRotation();
			}
}

void UMilitaryBaseComp::Server_GetMilitaryBaseSpawnPoint_Implementation()
{
	GetMilitaryBaseSpawnPoint();
}


void UMilitaryBaseComp::Server_SpawnModule_Implementation(AMR_General* OwningPawn)
{
	SpawnModules(OwningPawn);
}

void UMilitaryBaseComp::SpawnUnit(AMR_General* InstigatorPawn, AMilitaryBase* BaseInstance, AModuleActor* Module)
{
	if (!General->HasAuthority())
	{
		Server_SpawnUnit(InstigatorPawn, BaseInstance, Module);
	}
	if (InstigatorPawn && BaseInstance && Module)
	{
		UBuildingDataAsset* BuildingDataAsset = Module->BuildingDataAsset;
		if (BuildingDataAsset)
		{
			TSubclassOf<AUnit> UnitClassToSpawn = BuildingDataAsset->UnitToSpawn;
			TObjectPtr<UWorld> World = GetWorld();

			FVector UnitSpawnLocation = BaseInstance->SpawnPoint_Ground->GetComponentLocation();
			FRotator UnitSpawnRotation = BaseInstance->SpawnPoint_Ground->GetComponentRotation();
			FActorSpawnParameters UnitSpawnParams;
			UnitSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			UnitSpawnParams.Owner = InstigatorPawn;
			UnitSpawnParams.Instigator = InstigatorPawn;
			
			AUnit* UnitInstance = World->SpawnActor<AUnit>(UnitClassToSpawn, UnitSpawnLocation, UnitSpawnRotation, UnitSpawnParams);
			
		}
	}
	DBG(3, "MBC: NOT INstigator || BaseInstance || Module")
}

void UMilitaryBaseComp::Server_SpawnUnit_Implementation(AMR_General* InstigatorPawn, AMilitaryBase* BaseInstance,
	AModuleActor* Module)
{
	SpawnUnit(InstigatorPawn, BaseInstance, Module);
}


/*void UMilitaryBaseComp::SpawnUnit()
{
	DBG(3, "Spawnunit");
	if (!GetOwner()->HasAuthority())
	{
		Server_SpawnUnit();
		return;
	}

	if (General && General->BaseInstance && General->BaseInstance->SpawnPoint_Ground)
	{
		FVector Location = General->BaseInstance->SpawnPoint_Ground->GetComponentLocation();
        FRotator Rotation = General->BaseInstance->SpawnPoint_Ground->GetComponentRotation();
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = General->GetPlayerState()->GetOwningController();
		SpawnParams.Instigator = General;

		/// for testing without VR only!!!!
		/*UnitToSpawn = General->DefaultUnit;
		AUnit* SpawnedUnitTest = GetWorld()->SpawnActor<AUnit>(UnitToSpawn, Location, Rotation, SpawnParams);#1#
		// SpawnedUnitTest->Body->SetMaterial(0, General->PlayerDefaultColor);
		/// for testing without VR only!!!!
    
        if (General->CurrentlySelectedModule)
        {
            UnitToSpawn = General->CurrentlySelectedModule->BuildingDataAsset->UnitToSpawn;
            if (!UnitToSpawn)
            {
            	DBG(3, "MBC: No Unit selected") 
            }
            else
            {
	            if (HasEnoughResources())
	            {
		            AUnit* SpawnedUnit = GetWorld()->SpawnActor<AUnit>(UnitToSpawn, Location, Rotation, SpawnParams);
		            General->ResourcesComponent->UpdateResources(General->CurrentlySelectedModule->BuildingDataAsset->UnitToSpawnData->Price);
	            }
	            else
	            {
		            DBG(3, "NOT ENOUGH RESOURCES");
	            }
            }
        }
		DBG(5, "MBC: Module not selected!") 
	}
	else
	{
		DBG(5, "MBC: No MB or SpawnPoint!") 
	}
}

void UMilitaryBaseComp::Server_SpawnUnit_Implementation()
{
	SpawnUnit();
}

bool UMilitaryBaseComp::HasEnoughResources() const
{
	if (General && General->ResourcesComponent)
	{
		float AvailableResources = General->ResourcesComponent->AvailableResources;
		
		if (UnitToSpawn && General->CurrentlySelectedModule->BuildingDataAsset)
		{
			return AvailableResources >= General->CurrentlySelectedModule->BuildingDataAsset->UnitToSpawnData->Price;
		}
	}
	return false;
}*/
