#include "HealthBarActor.h"

#include "Components/WidgetComponent.h"


AHealthBarActor::AHealthBarActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Widget = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");
}

void AHealthBarActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthBarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

