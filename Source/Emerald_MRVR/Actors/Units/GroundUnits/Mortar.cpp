#include "Mortar.h"

#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"


AMortar::AMortar()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMortar::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AMortar::OnActorOverlap);
	BeginTransformation();
}

void AMortar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/* Transformation to Mine Timer Function*/
void AMortar::BeginTransformation()
{
	float TransformationStart = FMath::RandRange(TransformationStartMin, TransformationStartMax);
	GetWorld()->GetTimerManager().SetTimer(BeginTransformationHandle, this, &AMortar::TransformToMine, TransformationStart, false);
}

/* Transformation to Mine form and starts autodestruction Timer */
void AMortar::TransformToMine()
{
	// Play transform animation
	if (!TransformedForm || !UnitMovementComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMortar::!TransformedForm || !UnitMovementComponent"));
		return;
	}
	
	Body->SetStaticMesh(TransformedForm);
	UnitMovementComponent->DestroyComponent();
	bIsAttacker = true;
	Strenght = 4.f;

	GetWorld()->GetTimerManager().SetTimer(AutodestructionHandle, this, &AMortar::Autodestruction, AutodestructionTime, false);
}

void AMortar::OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Explode(OtherActor);
}

void AMortar::Explode(AActor* OtherActor)
{
	AUnit* Unit = Cast<AUnit>(OtherActor);
	if (!Unit)
	{
		return;
	}
	
	if (Unit->GetOwner() != GetOwner())
	{
		// Play Explosion
		if (Unit->Strenght < MineStrenght)
		{
			Unit->KillMe();
			KillMe();
		}
	}
}

/* Destroy self after Autodestruction Time when it doesn't overlap with enemy unit */
void AMortar::Autodestruction()
{
	KillMe();
}
