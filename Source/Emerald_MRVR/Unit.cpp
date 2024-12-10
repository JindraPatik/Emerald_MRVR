#include "Unit.h"

#include "DebugMacros.h"
#include "SphereComponent.h"
#include "Components/MilitaryBaseComp.h"
#include "Components/UnitMovementComponent.h"
#include "CORE/MR_General.h"
#include "CORE/PC_MR_General.h"
#include "Data/BuildingDataAsset.h"
#include "Data/UnitDataAsset.h"
#include "Net/UnrealNetwork.h"


AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	Body->SetIsReplicated(true);
	
	RootComponent = Body;
	bReplicates = true;
	bNetLoadOnClient = true;
	
	UnitMovementComponent = CreateDefaultSubobject<UUnitMovementComponent>("UnitMovementComponent");
	UnitMovementComponent->SetIsReplicated(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(Body);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

		AMR_General* General = Cast<AMR_General>(GetInstigator());
		if (General)
		{
			Body->SetMaterial(0, General->PlayerDefaultColor);	
		}
		else
		{
			DBG(3, "AUnit failed to cast GENERAL")
		}
}

void AUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUnit, Speed);
	DOREPLIFETIME(AUnit, Strenght);
	DOREPLIFETIME(AUnit, Price);
	DOREPLIFETIME(AUnit, Body);
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

/*void AUnit::SetUnitStats(float InSpeed, float InPrice, float InStrenght)
{
	Speed = InSpeed;
	Price = InPrice;
	Strenght = InPrice;
}*/

