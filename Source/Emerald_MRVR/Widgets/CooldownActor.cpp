#include "CooldownActor.h"

#include "Components/WidgetComponent.h"

ACooldownActor::ACooldownActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this,"RootComp");
	RootComponent = RootComp;
	
	Widget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this,"CD_Widget");
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

