#include "Unit.h"

#include "SphereComponent.h"
#include "Components/MilitaryBaseComp.h"
#include "Components/UnitMovementComponent.h"
#include "CORE/MR_General.h"
#include "Data/UnitDataAsset.h"
#include "Net/UnrealNetwork.h"


AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	RootComponent = Body;
	bReplicates = true;
	UnitMovementComponent = CreateDefaultSubobject<UUnitMovementComponent>("UnitMovementComponent");

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(Body);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

	SetUnitStats();

}

void AUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUnit, Speed)
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UnitMovementComponent->bMovementEnabled)
	{
		UnitMovementComponent->MoveTo(DeltaTime);	
	}

}

void AUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUnit::SetUnitStats()
{
	AMR_General* General = Cast<AMR_General>(GetOwner());
	if (General && General->MilitaryBaseComp->SelectedUnit)
	{
		UUnitDataAsset* UnitStats = General->MilitaryBaseComp->SelectedUnit;
		Speed = UnitStats->Speed;
		Price = UnitStats->Price;
		Strenght = UnitStats->Strength;
	}
}

