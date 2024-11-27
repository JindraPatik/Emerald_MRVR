#include "MilitaryBaseComp.h"

#include "BuildingsModuleComponent.h"
#include "ResourcesComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/EK_GameMode.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/Data/BuildingDataAsset.h"
#include "Engine/TargetPoint.h"
#include "Net/UnrealNetwork.h"


UMilitaryBaseComp::UMilitaryBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	TargetPoint = CreateDefaultSubobject<ATargetPoint>("MilitaryBaseTargetPoint");
	SetIsReplicatedByDefault(true);
}

void UMilitaryBaseComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMilitaryBaseComp, General);
	DOREPLIFETIME(UMilitaryBaseComp, UnitTargetLoc);
}


void UMilitaryBaseComp::BeginPlay()
{
	Super::BeginPlay();
	General = Cast<AMR_General>(GetOwner()->GetOwner());
}


void UMilitaryBaseComp::SelectUnitToSpawn(UBuildingDataAsset* SelectedBuilding)
{
	SelectedUnit = SelectedBuilding->UnitToSpawnData;
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

	AEK_GameMode* GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is invalid!"));
		return;
	}

	if (ensure(GameMode) && (GameMode->TargetPoints.Num() > 0))
	{
		TargetPoint = GameMode->TargetPoints.IsValidIndex(0) ? GameMode->TargetPoints[0] : nullptr;
		SpawnPoint = GameMode->TargetPoints[0]->GetTransform();
		GameMode->TargetPoints.RemoveAt(0);

		if (TargetPoint)
		{
			FVector SpawnLocation = TargetPoint->GetActorLocation();
			FRotator SpawnRotation = TargetPoint->GetActorRotation();
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = OwningPawn;
			SpawnParameters.Owner = OwningPawn;

			if (General)
			{
				// Spawn Base Instance
				General->BaseInstance = GetWorld()->SpawnActor<AMilitaryBase>(MilitaryBase, SpawnLocation, SpawnRotation, SpawnParameters);
			}
		}
	}
}
void UMilitaryBaseComp::Server_SpawnMilitaryBase_Implementation(AMR_General* OwningPawn)
{
	SpawnMilitaryBase(OwningPawn);
}

void UMilitaryBaseComp::SpawnUnit()
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SpawnUnit();
	}

	if (General && General->BaseInstance && General->BaseInstance->SpawnPoint_Ground)
	{

		FVector Location = General->BaseInstance->SpawnPoint_Ground->GetComponentLocation();
        FRotator Rotation = General->BaseInstance->SpawnPoint_Ground->GetComponentRotation();
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = General->GetController();
    
        if (General->CurrentlySelectedModule)
        {
            UnitToSpawn = General->CurrentlySelectedModule->BuildingDataAsset->UnitToSpawn;
            if (!UnitToSpawn)
            {
                DBG(3, "MBC: No Unit to Spawn") 
            }
            else
            {
                AUnit* SpawnedUnit = GetWorld()->SpawnActor<AUnit>(UnitToSpawn, Location, Rotation, SpawnParams);
            	SpawnedUnit->Body->SetMaterial(0, General->PlayerDefaultColor);
            }
        }

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
	// Dodelat po nacteni z DataAssetu;
	if (General && General->ResourcesComponent)
	{
		float AvailableResources = General->ResourcesComponent->AvailableResources;
	}
	return true;
}