#include "MilitaryBaseComp.h"

#include "EngineUtils.h"
#include "ResourcesComponent.h"
#include "UnitMovementComponent.h"
#include "Animation/WidgetAnimation.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/ModuleActor.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/CORE/EK_GameMode.h"
#include "Emerald_MRVR/CORE/Gamemode_Single.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


class AEKGameState;

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
	DOREPLIFETIME(UMilitaryBaseComp, MyBaseInstance);
	DOREPLIFETIME(UMilitaryBaseComp, UnitSpawnLocation);
	DOREPLIFETIME(UMilitaryBaseComp, UnitSpawnRotation);
}

void UMilitaryBaseComp::BeginPlay()
{
	Super::BeginPlay();
	
	
	MyOwner = Cast<APawn>(GetOwner());
	ResourcesComponentInst = Cast<UResourcesComponent>(GetOwner()->FindComponentByClass<UResourcesComponent>());
}

void UMilitaryBaseComp::SetSpawnPointForBase()
{
	if (!GetOwner()->HasAuthority())
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
	if (!OwningPawn->HasAuthority())
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
			ResourcesComponentInst->StartGrowResources();
		}
	}	
}

void UMilitaryBaseComp::Server_SpawnMilitaryBase_Implementation(APawn* InOwner)
{
	SpawnMilitaryBase(InOwner);
}


void UMilitaryBaseComp::SpawnModules(APawn* OwningPawn)
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
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
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
							ModuleInstance->SetReplicates(true);

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

void UMilitaryBaseComp::SpawnUnit(APawn* InstigatorPawn, AModuleActor* Module)
{
	if (!GetOwner()->HasAuthority())
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
			UWorld* World = GetWorld();

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
		}
	}
}


void UMilitaryBaseComp::Server_SpawnUnit_Implementation(APawn* InstigatorPawn, AModuleActor* Module)
{
	SpawnUnit(InstigatorPawn, Module);
}


bool UMilitaryBaseComp::HasEnoughResources(UBuildingDataAsset* BuildingDataAsset)
{
		if (!GetOwner()->HasAuthority())
		{
			Server_HasEnoughResources(BuildingDataAsset);
		}
	
		if (ResourcesComponentInst)
		{
			if (ResourcesComponentInst->AvailableResources >= BuildingDataAsset->UnitToSpawnData->Price)
			{
				return true;
			}
			SpawnNotEnoughResWidget();
			return false;
		}
		return false;
}

void UMilitaryBaseComp::Server_HasEnoughResources_Implementation(UBuildingDataAsset* BuildingDataAsset)
{
	HasEnoughResources(BuildingDataAsset);
}

void UMilitaryBaseComp::SpawnNotEnoughResWidget()
{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		FVector Location = FVector(0.f, 0.f, 225.f);
		FRotator Rotation = FRotator::ZeroRotator;
		NotEnoughResInstance = GetWorld()->SpawnActor<AActor>(NotEnoughResourcesWidgetActor, Location, Rotation, SpawnParameters);
		UGameplayStatics::PlaySound2D(this, NotEnoughResSound, 1.f);
}


