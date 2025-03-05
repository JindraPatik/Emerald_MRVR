#include "MilitaryStationComponent.h"

#include "EngineUtils.h"
#include "Emerald_MRVR/Actors/MilitaryStation/Building.h"
#include "Emerald_MRVR/Actors/MilitaryStation/MilitaryStation.h"
#include "Emerald_MRVR/Actors/Support/SpawnPointStation.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Emerald_MRVR/Components/Resources/ResourcesComponent.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Emerald_MRVR/Data/UnitDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


class AEKGameState;

UMilitaryStationComp::UMilitaryStationComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	SpawnPointForMilitaryStation = CreateDefaultSubobject<ASpawnPointStation>("MilitaryStationTargetPoint");
}

void UMilitaryStationComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMilitaryStationComp, AvailableBuildings);
	DOREPLIFETIME(UMilitaryStationComp, PlayerMilitaryStationInstance);
	DOREPLIFETIME(UMilitaryStationComp, UnitSpawnLocation);
	DOREPLIFETIME(UMilitaryStationComp, UnitSpawnRotation);
	DOREPLIFETIME(UMilitaryStationComp, UnitInstance);
}

void UMilitaryStationComp::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPawn = Cast<APawn>(GetOwner());
	ResourcesComponentInst = Cast<UResourcesComponent>(GetOwner()->FindComponentByClass<UResourcesComponent>());
	
}

void UMilitaryStationComp::SetSpawnPointForStation()
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SetSpawnPointForBase();
		return;
	}
	
	//pb: potreboval bych vedet, jaka je zamyslena logika spawn pointu - zatim to takto nema vyznam, protoze se vybere vzdy ten prvni nalezeny
	TArray<ASpawnPointStation*> AllSpawnPointsStation;
	
	for (TActorIterator<ASpawnPointStation> It(GetWorld()); It; ++It)
	{
		ASpawnPointStation* SpawnPointStation = *It;
		if (!SpawnPointStation)
		{
			return;
		}
		AllSpawnPointsStation.Add(SpawnPointStation);
	}

	Algo::Sort(AllSpawnPointsStation, [](const ASpawnPointStation* A, const ASpawnPointStation* B) {
	return A->index < B->index;
	});
	
	if (AllSpawnPointsStation.Num() > 0)
	{
		SpawnPointForMilitaryStation = AllSpawnPointsStation[0];
		AllSpawnPointsStation[0]->Destroy();
	}
}


void UMilitaryStationComp::Server_SetSpawnPointForBase_Implementation()
{
	SetSpawnPointForStation();
}


void UMilitaryStationComp::SpawnMilitaryStation(APawn* InPawn)
{
	if (!InPawn->HasAuthority())
	{
		Server_SpawnMilitaryStation(InPawn);
		return;
	}
	
	SetSpawnPointForStation();
	
	if (!SpawnPointForMilitaryStation)
	{
		UE_LOG(LogTemp, Warning, TEXT("MilitaryStationComponent: !SpawnPointForMilitaryStation !!!!"))
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = InPawn;
	SpawnParameters.Owner = InPawn;
	FVector SpawnLocation = SpawnPointForMilitaryStation->GetActorLocation();
	FRotator SpawnRotation = SpawnPointForMilitaryStation->GetActorRotation();

	/* Set Tag in editor !!! */
	SpawnPointForMilitaryStation->bIsReversed ? bIsReversed = true : bIsReversed = false; // Set reversed bool
			
	if (!InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("MilitaryStationComponent: !InPawn"))
		return;
	}

	PlayerMilitaryStationInstance = GetWorld()->SpawnActor<AMilitaryStation>(MilitaryStation, SpawnLocation, SpawnRotation, SpawnParameters);
	UE_LOG(LogTemp, Warning, TEXT("MilitaryStationComponent: SpawnLocation %s"), *SpawnLocation.ToString());
		
}

void UMilitaryStationComp::Server_SpawnMilitaryStation_Implementation(APawn* InPawn)
{
	SpawnMilitaryStation(InPawn);
}


void UMilitaryStationComp::SpawnBuildings(APawn* InPawn)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SpawnBuildings(InPawn);
		return;
	}

	if (!SpawnPointForMilitaryStation)
	{
		return;
	}
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = InPawn;
	SpawnParameters.Owner = InPawn;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (AvailableBuildings.Num() > 0)
	{
		for (UBuildingDataAsset* Building : AvailableBuildings)
		{
			if (PlayerMilitaryStationInstance && PlayerMilitaryStationInstance->BuildingPositions.Num() >= 0)
			{
				for (USceneComponent* BuildingPosition : PlayerMilitaryStationInstance->BuildingPositions)
				{
					if (BuildingPosition->ComponentHasTag(Building->BuildingName)) // Nastavit v Editoru!
					{
						BuildingSpawnLoc = BuildingPosition->GetComponentLocation();
						BuildingSpawnRot = BuildingPosition->GetComponentRotation();

						if (!SpawnPointForMilitaryStation)
						{
							return;
						}

						/* Overrides Location and Rotation set in editor */
						for (USceneComponent* BuildingPositionEditorOverride : SpawnPointForMilitaryStation->SpawnPointComponents)
						{
							if (BuildingPositionEditorOverride->ComponentHasTag(Building->BuildingName))
							{
								BuildingSpawnLoc = BuildingPositionEditorOverride->GetComponentLocation();
								BuildingSpawnRot = BuildingPositionEditorOverride->GetComponentRotation();
								break;
							}
						}
						
						ABuilding* BuildingInstance = GetWorld()->SpawnActor<ABuilding>(Building->BuildingClass, BuildingSpawnLoc, BuildingSpawnRot, SpawnParameters);
						AvailableBuildingsActors.Add(BuildingInstance);

						if (!BuildingInstance)
						{
							return;
						}
						
						BuildingInstance->BuildingDataAsset = Building;
						BuildingInstance->SpawnInfoWidget();
						BuildingInstance->SpawnCooldownWidget();
						BuildingInstance->SetReplicates(true);
					}
				}
			}
		}
	}
}

void UMilitaryStationComp::Server_SpawnBuildings_Implementation(APawn* InPawn)
{
	SpawnBuildings(InPawn);
}

AUnit* UMilitaryStationComp::SpawnUnit(APawn* InstigatorPawn, ABuilding* Building)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SpawnUnit(InstigatorPawn, Building);
		return nullptr;
	}
	
	if (InstigatorPawn && PlayerMilitaryStationInstance && Building)
	{
		UBuildingDataAsset* BuildingDataAsset = Building->BuildingDataAsset;
		if (BuildingDataAsset)
		{
			TSubclassOf<AUnit> UnitClassToSpawn = BuildingDataAsset->UnitToSpawn;
			UWorld* World = GetWorld();

			if (!BuildingDataAsset->UnitToSpawnData->IsFlyingUnit)
			{
				UnitSpawnLocation = PlayerMilitaryStationInstance->SpawnPointGround->GetComponentLocation();
				UnitSpawnRotation = PlayerMilitaryStationInstance->SpawnPointGround->GetComponentRotation();
			}
			if (BuildingDataAsset->UnitToSpawnData->IsFlyingUnit)
			{
				UnitSpawnLocation = PlayerMilitaryStationInstance->SpawnPointAir->GetComponentLocation();
				UnitSpawnRotation = PlayerMilitaryStationInstance->SpawnPointAir->GetComponentRotation();
			}

			if (BuildingDataAsset->UnitToSpawnData->IsRocket)
			{
				UnitSpawnLocation = Building->GetActorLocation() + FVector(0,0,8.f);
				UnitSpawnRotation = FRotator(90.f, 0.f, 0.f);
			}
			
			FActorSpawnParameters UnitSpawnParams;
			UnitSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			UnitSpawnParams.Owner = GetOwner();
			UnitSpawnParams.Instigator = InstigatorPawn;


			if (HasEnoughResources(BuildingDataAsset) && Building->bSpawningEnabled)
			{
				Building->bSpawningEnabled = false;
				UUnitDataAsset* SpawnedUnitDataAsset = BuildingDataAsset->UnitToSpawnData;
				Building->Cooldown(BuildingDataAsset->Cooldown);
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
					UnitInstance->OwningBuilding = Building;
					UnitInstance->HasReturnPoint = SpawnedUnitDataAsset->HasReturnPoint;
					
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


void UMilitaryStationComp::Server_SpawnUnit_Implementation(APawn* InstigatorPawn, ABuilding* Building)
{
	SpawnUnit(InstigatorPawn, Building);
}


bool UMilitaryStationComp::HasEnoughResources(UBuildingDataAsset* BuildingDataAsset)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_HasEnoughResources(BuildingDataAsset);
	}

	if (ResourcesComponentInst && ResourcesComponentInst->AvailableResources >= BuildingDataAsset->UnitToSpawnData->Price)
	{
		return true;
	}

	SpawnNotEnoughResWidget();
	return false;
}

void UMilitaryStationComp::Server_HasEnoughResources_Implementation(UBuildingDataAsset* BuildingDataAsset)
{
	HasEnoughResources(BuildingDataAsset);
}

void UMilitaryStationComp::SpawnNotEnoughResWidget()
{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		FVector Location = FVector(0.f, 0.f, 225.f);
		FRotator Rotation = FRotator::ZeroRotator;
		NotEnoughResInstance = GetWorld()->SpawnActor<AActor>(NotEnoughResourcesWidgetActor, Location, Rotation, SpawnParameters);
		UGameplayStatics::PlaySound2D(this, NotEnoughResSound, 1.f);
}


