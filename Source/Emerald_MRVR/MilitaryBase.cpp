

#include "MilitaryBase.h"
#include "BoxComponent.h"


AMilitaryBase::AMilitaryBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseBody = CreateDefaultSubobject<UStaticMeshComponent>("BaseBody");
	BaseBox = CreateDefaultSubobject<UBoxComponent>("BaseBox");
}

void AMilitaryBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMilitaryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

