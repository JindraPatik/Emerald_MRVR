#include "Unit.h"

#include "AIHelpers.h"
#include "BoxComponent.h"
#include "DebugMacros.h"
#include "SphereComponent.h"
#include "Components/CombatComponent.h"
#include "Components/DownScaleComponent.h"
#include "Components/MilitaryBaseComp.h"
#include "Components/UnitMovementComponent.h"
#include "CORE/MR_General.h"
#include "CORE/PC_MR_General.h"
#include "CORE/UnitAIController.h"
#include "Data/BuildingDataAsset.h"
#include "Data/UnitDataAsset.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/AIModule/Classes/AIController.h"


AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	Body->SetIsReplicated(true);
	Body->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	RootComponent = Body;
	bReplicates = true;
	bNetLoadOnClient = true;

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	
	UnitMovementComponent = CreateDefaultSubobject<UUnitMovementComponent>("UnitMovementComponent");
	UnitMovementComponent->SetIsReplicated(true);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>("CombatComponent");

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(Body);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_Pawn);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
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
	DOREPLIFETIME(AUnit, Damage);
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

