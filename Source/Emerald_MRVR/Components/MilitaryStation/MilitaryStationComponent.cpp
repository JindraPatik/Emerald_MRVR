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

UMilitaryStationComponent::UMilitaryStationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SpawnPointForMilitaryStation = CreateDefaultSubobject<ASpawnPointStation>("MilitaryStationTargetPoint");
}

void UMilitaryStationComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMilitaryStationComponent, AvailableBuildings);
	DOREPLIFETIME(UMilitaryStationComponent, MilitaryStationInstance);
	DOREPLIFETIME(UMilitaryStationComponent, UnitSpawnLocation);
	DOREPLIFETIME(UMilitaryStationComponent, UnitSpawnRotation);
	DOREPLIFETIME(UMilitaryStationComponent, UnitInstance);
}

void UMilitaryStationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwningPawn = Cast<APawn>(GetOwner());
	ResourcesComponentInst = Cast<UResourcesComponent>(GetOwner()->FindComponentByClass<UResourcesComponent>());
	
}

void UMilitaryStationComponent::SetSpawnPointForStation()
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


void UMilitaryStationComponent::Server_SetSpawnPointForBase_Implementation()
{
	SetSpawnPointForStation();
}


void UMilitaryStationComponent::SpawnMilitaryStation(APawn* InPawn)
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
	SpawnParameters.Owner = GetOwner();
	FVector SpawnLocation = SpawnPointForMilitaryStation->GetActorLocation();
	FRotator SpawnRotation = SpawnPointForMilitaryStation->GetActorRotation();

	/* Set Tag in editor !!! */
	SpawnPointForMilitaryStation->bIsReversed ? bIsReversed = true : bIsReversed = false; // Set reversed bool
			
	if (!InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("MilitaryStationComponent: !InPawn"))
		return;
	}

	MilitaryStationInstance = GetWorld()->SpawnActor<AMilitaryStation>(MilitaryStation, SpawnLocation, SpawnRotation, SpawnParameters);
	UE_LOG(LogTemp, Warning, TEXT("MilitaryStationComponent: SpawnLocation %s"), *SpawnLocation.ToString());
		
}

void UMilitaryStationComponent::Server_SpawnMilitaryStation_Implementation(APawn* InPawn)
{
	SpawnMilitaryStation(InPawn);
}


void UMilitaryStationComponent::SpawnBuildings(APawn* InPawn)
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
			if (MilitaryStationInstance && MilitaryStationInstance->BuildingPositions.Num() >= 0)
			{
				for (USceneComponent* BuildingPosition : MilitaryStationInstance->BuildingPositions)
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

void UMilitaryStationComponent::Server_SpawnBuildings_Implementation(APawn* InPawn)
{
	SpawnBuildings(InPawn);
}

AUnit* UMilitaryStationComponent::SpawnUnit(APawn* InstigatorPawn, ABuilding* Building)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SpawnUnit(InstigatorPawn, Building);
		return nullptr;
	}
	
	if (InstigatorPawn && MilitaryStationInstance && Building)
	{
		UBuildingDataAsset* BuildingDataAsset = Building->BuildingDataAsset;
		if (BuildingDataAsset)
		{
			TSubclassOf<AUnit> UnitClassToSpawn = BuildingDataAsset->UnitToSpawn;
			UWorld* World = GetWorld();

			if (!BuildingDataAsset->UnitToSpawnData->IsFlyingUnit)
			{
				UnitSpawnLocation = MilitaryStationInstance->SpawnPointGround->GetComponentLocation();
				UnitSpawnRotation = MilitaryStationInstance->SpawnPointGround->GetComponentRotation();
			}
			if (BuildingDataAsset->UnitToSpawnData->IsFlyingUnit)
			{
				UnitSpawnLocation = MilitaryStationInstance->SpawnPointAir->GetComponentLocation();
				UnitSpawnRotation = MilitaryStationInstance->SpawnPointAir->GetComponentRotation();
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


void UMilitaryStationComponent::Server_SpawnUnit_Implementation(APawn* InstigatorPawn, ABuilding* Building)
{
	SpawnUnit(InstigatorPawn, Building);
}


bool UMilitaryStationComponent::HasEnoughResources(UBuildingDataAsset* BuildingDataAsset)
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

void UMilitaryStationComponent::Server_HasEnoughResources_Implementation(UBuildingDataAsset* BuildingDataAsset)
{
	HasEnoughResources(BuildingDataAsset);
}

void UMilitaryStationComponent::SpawnNotEnoughResWidget()
{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		FVector Location = FVector(0.f, 0.f, 225.f);
		FRotator Rotation = FRotator::ZeroRotator;
		NotEnoughResInstance = GetWorld()->SpawnActor<AActor>(NotEnoughResourcesWidgetActor, Location, Rotation, SpawnParameters);
		UGameplayStatics::PlaySound2D(this, NotEnoughResSound, 1.f);
}


