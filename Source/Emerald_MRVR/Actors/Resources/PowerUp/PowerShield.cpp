#include "PowerShield.h"

#include "Emerald_MRVR/Actors/MilitaryStation/MilitaryStation.h"
#include "Emerald_MRVR/Components/MilitaryStation/MilitaryStationComponent.h"
#include "PowerSubActors/ShieldActor.h"

APowerShield::APowerShield()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APowerShield::BeginPlay()
{
	Super::BeginPlay();
	
}

void APowerShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APowerShield::Activate()
{
	Super::Activate();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!Owner)
	{
		return;
	}

	UMilitaryStationComponent* MilitaryStationComponent = Owner->FindComponentByClass<UMilitaryStationComponent>();
	if (!MilitaryStationComponent)
	{
		return;
	}

	if (!MilitaryStationComponent->MilitaryStationInstance)
	{
		return;
	}
	
	FVector Location = MilitaryStationComponent->MilitaryStationInstance->GetActorLocation();
	FRotator Rotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AShieldActor* ShieldInstance = World->SpawnActor<AShieldActor>(ShieldActorClass, Location, Rotation, SpawnParameters);
}
