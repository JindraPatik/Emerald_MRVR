#include "Unit.h"
#include "BoxComponent.h"
#include "Components/DownScaleComponent.h"
#include "Components/UnitMovementComponent.h"
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
	// Body->SetGenerateOverlapEvents(true);
	
	bReplicates = true;
	bNetLoadOnClient = true;

	DownScaleComponent = CreateDefaultSubobject<UDownScaleComponent>("DownscaleComponent");

	// Collision
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_Pawn);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); 
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->IgnoreActorWhenMoving(GetOwner(), true);
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();
	UnitMovementComponent = FindComponentByClass<UUnitMovementComponent>();

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

void AUnit::KillMe_Implementation()
{
	Destroy();
}

void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
