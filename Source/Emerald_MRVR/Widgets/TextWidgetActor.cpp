#include "TextWidgetActor.h"

#include "Components/WidgetComponent.h"


ATextWidgetActor::ATextWidgetActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Widget = CreateDefaultSubobject<UWidgetComponent>("TextWidget");
}

void ATextWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATextWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

