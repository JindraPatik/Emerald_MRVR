#include "CooldownActor.h"

#include "Components/WidgetComponent.h"

ACooldownActor::ACooldownActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	RootComponent = RootComp;
	
	Widget = CreateDefaultSubobject<UWidgetComponent>("CD_Widget");
	Widget->SetupAttachment(RootComp);

}
void ACooldownActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
	
}

void ACooldownActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

