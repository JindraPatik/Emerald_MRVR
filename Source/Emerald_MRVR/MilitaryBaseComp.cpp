#include "MilitaryBaseComp.h"

#include "AsyncTreeDifferences.h"
#include "DebugMacros.h"
#include "MilitaryBase.h"
#include "Unit.h"
#include "CORE/EK_GameMode.h"
#include "CORE/MR_General.h"
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
}

void UMilitaryBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMilitaryBaseComp::SpawnMilitaryBase(AMR_General* OwningPawn)
{
	if (!OwningPawn->HasAuthority())
	{
		// Pokud nejsme na serveru, požádej server, aby provedl spawn
		Server_SpawnMilitaryBase(OwningPawn);
		return; // Ukonči funkci na klientovi
	}

	// Pokud jsme na serveru, pokračujeme zde
	AEK_GameMode* GameMode = Cast<AEK_GameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is invalid!"));
		return;
	}

	//TArray<ATargetPoint*> TargetPoints = GameMode->GetAllTargetpoints();
	if (GameMode && (GameMode->TargetPoints.Num() > 0))
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
				// Provádíme spawn pouze na serveru
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




