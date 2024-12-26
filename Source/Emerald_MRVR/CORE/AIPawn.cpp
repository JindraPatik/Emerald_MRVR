#include "AIPawn.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Emerald_MRVR/Components/AI_Component.h"
#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/Components/MilitaryBaseComp.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"

AAIPawn::AAIPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>("Resources");
	MilitaryBaseComp = CreateDefaultSubobject<UMilitaryBaseComp>("MilitaryBaseComp");
	AI_Component = CreateDefaultSubobject<UAI_Component>("AI_Component");

}

void AAIPawn::BeginPlay()
{
	Super::BeginPlay();
	
	MilitaryBaseComp->SpawnMilitaryBase(this);
	MilitaryBaseComp->SpawnModules(this);
}


void AAIPawn::Tick(float DeltaTime)
{

}

