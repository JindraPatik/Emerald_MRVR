#include "AIPawn.h"

#include "Emerald_MRVR/Components/HealthComponent.h"
#include "Emerald_MRVR/Components/MilitaryBaseComp.h"
#include "Emerald_MRVR/Components/ResourcesComponent.h"

AAIPawn::AAIPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	/*HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health");
	
	ResourcesComponent = CreateDefaultSubobject<UResourcesComponent>("Resources");
	
	MilitaryBaseComp = CreateDefaultSubobject<UMilitaryBaseComp>("MilitaryBaseComp");*/


}

void AAIPawn::BeginPlay()
{
	
}

void AAIPawn::Tick(float DeltaTime)
{

}

void AAIPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

