#include "Unit.h"

#include "BoxComponent.h"
#include "DebugMacros.h"
#include "Components/CombatComponent.h"
#include "Components/DownScaleComponent.h"
#include "Components/UnitMovementComponent.h"
#include "CORE/MR_General.h"
#include "Net/UnrealNetwork.h"


AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	UnitRoot = CreateDefaultSubobject<USceneComponent>("UnitRoot");
	UnitRoot->SetIsReplicated(true);
	SetRootComponent(UnitRoot);
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	Body->SetIsReplicated(true);
	Body->SetCollisionResponseToAllChannels(ECR_Ignore);
	Body->SetupAttachment(UnitRoot);
	
	bReplicates = true;
	bNetLoadOnClient = true;

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");
	
	UnitMovementComponent = CreateDefaultSubobject<UUnitMovementComponent>("UnitMovementComponent");
	UnitMovementComponent->SetIsReplicated(true);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>("CombatComponent");

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(UnitRoot);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_Pawn);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();

}

void AUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUnit, Speed);
	DOREPLIFETIME(AUnit, Strenght);
	DOREPLIFETIME(AUnit, Price);
	DOREPLIFETIME(AUnit, Body);
	DOREPLIFETIME(AUnit, Damage);
	DOREPLIFETIME(AUnit, UnitRoot);
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

