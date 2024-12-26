#include "AI_Component.h"

#include "CrystalSpawnerComp.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Emerald_MRVR/CORE/Gamemode_Single.h"


UAI_Component::UAI_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAI_Component::BeginPlay()
{
	Super::BeginPlay();
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
		 }
	}
}

void UAI_Component::PostInitProperties()
{
	Super::PostInitProperties();
}	

void UAI_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAI_Component::OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst)
{
	DBG(5.f, "BINDINGS WORKS! BINDINGS WORKS! BINDINGS WORKS! BINDINGS WORKS! ")
}



