#include "MilitaryBaseComp.h"

#include "AsyncTreeDifferences.h"
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
	SetIsReplicatedByDefault(true);
	General = Cast<AMR_General>(GetOwner());
	// UDELAT EVENTABY SE NACETLO PO NASPAWNOVANI BASE
	// AvailableModules = General->BaseInstance->BuildingsModuleComponent->AvailableBuildings;
}


void UMilitaryBaseComp::SelectUnitToSpawn(UBuildingDataAsset* SelectedBuilding)
{
	SelectedUnit = SelectedBuilding->UnitToSpawn;
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

	// TArray<ATargetPoint*> TargetPoints = GameMode->GetAllTargetpoints();
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
        
        	if (!UnitToSpawn)
        	{
        		DBG_5S("MBC: No unit set!")
        	}
        	else
        	{
        		GetWorld()->SpawnActor<AUnit>(UnitToSpawn, Location, Rotation, SpawnParams);
        	}
	}
	else
	{
		DBG_5S("MBC: No MB or SpawnPoint!") 
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





