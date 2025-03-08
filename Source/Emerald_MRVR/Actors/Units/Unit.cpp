#include "Unit.h"

#include "Emerald_MRVR/Actors/MilitaryStation/Building.h"
#include "Emerald_MRVR/Components/DownScaleComponent.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"
#include "Net/UnrealNetwork.h"

AUnit::AUnit(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	UnitRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "UnitRoot");
	UnitRoot->SetIsReplicated(true);
	SetRootComponent(UnitRoot);
	
	Body = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this,"Body");
	Body->SetIsReplicated(true);
	Body->SetCollisionResponseToAllChannels(ECR_Ignore);
	Body->SetupAttachment(UnitRoot);
	Body->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Body->SetCollisionObjectType(ECC_Pawn);
	Body->SetCollisionResponseToAllChannels(ECR_Ignore);
	Body->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Body->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Body->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Body->SetGenerateOverlapEvents(true);
	
	bReplicates = true;
	bNetLoadOnClient = true;

	DownScaleComponent = ObjectInitializer.CreateDefaultSubobject<UDownScaleComponent>(this,"DownscaleComponent");
}

void AUnit::BeginPlay()
{
	Super::BeginPlay();
	UnitMovementComponent = FindComponentByClass<UUnitMovementComponent>();

	OnActorBeginOverlap.AddDynamic(this, &AUnit::DestroyOnReturn);

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

void AUnit::DestroyOnReturn(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ABuilding>(OtherActor) == OwningBuilding)
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Returned and destroyed"));
	}
}

void AUnit::AlreadyAttacked()
{
	bHasAttacked = true;
}


void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
