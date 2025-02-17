#include "MilitaryBaseComp.h"

#include "EngineUtils.h"
#include "ResourcesComponent.h"
#include "UnitMovementComponent.h"
#include "Emerald_MRVR/Actors/MilitaryBase.h"
#include "Emerald_MRVR/Actors/Building.h"
#include "Emerald_MRVR/Actors/Unit.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


class AEKGameState;

UMilitaryBaseComp::UMilitaryBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	//pb: tady by mohla byt ta zavislost na Editoru, kvuli ktere nejde zcookovat hra!
	//	ATargetPoint je  primarne editorovy helper objekt - viz. #if WITH_EDITORONLY_DATA v TargetPoint.h
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
	DOREPLIFETIME(UMilitaryBaseComp, UnitInstance);
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
	
	//pb: potreboval bych vedet, jaka je zamyslena logika spawn pointu - zatim to takto nema vyznam, protoze se vybere vzdy ten prvni nalezeny
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

		SpawnPointForMilitaryBase->ActorHasTag("Reversed") ? bIsReversed = true : bIsReversed = false; // Set reversed bool
				
		if (OwningPawn)
		{
			MyBaseInstance = GetWorld()->SpawnActor<AMilitaryBase>(MilitaryBase, SpawnLocation, SpawnRotation, SpawnParameters);
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
				if (MyBaseInstance && MyBaseInstance->ModulePositions.Num() >= 0)
				{
					for (USceneComponent* ModulePos : MyBaseInstance->ModulePositions)
					{
						if (ModulePos->ComponentHasTag(Module->BuildingName))
						{
							FVector ModuleSpawnLoc = ModulePos->GetComponentLocation();
							FRotator ModuleSpawnRot = ModulePos->GetComponentRotation();
							
							ABuilding* ModuleInstance = GetWorld()->SpawnActor<ABuilding>(Module->ModuleClass, ModuleSpawnLoc, ModuleSpawnRot, SpawnParameters);
							AvailableModulesActors.Add(ModuleInstance);
							if (ModuleInstance)
							{
								ModuleInstance->BuildingDataAsset = Module;
								ModuleInstance->SpawnInfoWidget();
								ModuleInstance->SpawnCooldownWidget();
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

AUnit* UMilitaryBaseComp::SpawnUnit(APawn* InstigatorPawn, ABuilding* Module)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SpawnUnit(InstigatorPawn, Module);
		return nullptr;
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
			if (BuildingDataAsset->UnitToSpawnData->IsFlyingUnit)
			{
				UnitSpawnLocation = MyBaseInstance->SpawnPoint_Air->GetComponentLocation();
				UnitSpawnRotation = MyBaseInstance->SpawnPoint_Air->GetComponentRotation();
			}

			if (BuildingDataAsset->UnitToSpawnData->IsRocket)
			{
				UnitSpawnLocation = Module->GetActorLocation() + FVector(0,0,8.f);
				UnitSpawnRotation = FRotator(90.f, 0.f, 0.f);
			}
			
			FActorSpawnParameters UnitSpawnParams;
			UnitSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			UnitSpawnParams.Owner = GetOwner();
			UnitSpawnParams.Instigator = InstigatorPawn;


			if (HasEnoughResources(BuildingDataAsset) && Module->bSpawningEnabled)
			{
				Module->bSpawningEnabled = false;
				UUnitDataAsset* SpawnedUnitDataAsset = BuildingDataAsset->UnitToSpawnData;
				Module->Cooldown(BuildingDataAsset->Cooldown);
				ResourcesComponentInst->UpdateResources(SpawnedUnitDataAsset->Price);
				UnitInstance = World->SpawnActor<AUnit>(UnitClassToSpawn, UnitSpawnLocation, UnitSpawnRotation, UnitSpawnParams);
				if (UnitInstance)
				{
					UnitInstance->Speed = SpawnedUnitDataAsset->Speed;
					UnitInstance->Price = SpawnedUnitDataAsset->Price;
					UnitInstance->Strenght = SpawnedUnitDataAsset->Strength;
					UnitInstance->Damage = SpawnedUnitDataAsset->Damage;
					UnitInstance->bIsFlyingUnit = SpawnedUnitDataAsset->IsFlyingUnit;
					UnitInstance->FightDelay = SpawnedUnitDataAsset->FightDelay;
					UnitInstance->bIsAttacker = SpawnedUnitDataAsset->IsAttacker;
					UnitInstance->bIsReversed = bIsReversed;
					UnitInstance->UnitSpawnLocation = UnitSpawnLocation;
					
					if (UnitInstance->UnitMovementComponent)
					{
						UnitInstance->UnitMovementComponent->UnitSpeed = SpawnedUnitDataAsset->Speed;
						UnitInstance->UnitMovementComponent->bIsFlying = SpawnedUnitDataAsset->IsFlyingUnit;
						UnitInstance->UnitMovementComponent->bIsReversed = bIsReversed;
						UnitInstance->UnitMovementComponent->FindPathPoints();
						UnitInstance->UnitMovementComponent->CreateMovementPath();
						UnitInstance->UnitMovementComponent->bMovementEnabled = true;
						
					}
					
					OnUnitSpawnedDelegate.Broadcast(UnitInstance, InstigatorPawn);
					return UnitInstance;
				}
			}
		}
	}
	return nullptr;
}


void UMilitaryBaseComp::Server_SpawnUnit_Implementation(APawn* InstigatorPawn, ABuilding* Module)
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


