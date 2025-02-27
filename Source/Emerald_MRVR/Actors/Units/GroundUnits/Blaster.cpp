#include "Blaster.h"

#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"


ABlaster::ABlaster()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABlaster::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ABlaster::OnActorOverlap);
	BeginTransformation();
}

void ABlaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/* Transformation to Mine Timer Function*/
void ABlaster::BeginTransformation()
{
	float TransformationStart = FMath::RandRange(TransformationStartMin, TransformationStartMax);
	GetWorld()->GetTimerManager().SetTimer(BeginTransformationHandle, this, &ABlaster::TransformToMine, TransformationStart, false);
}

/* Transformation to Mine form and starts autodestruction Timer */
void ABlaster::TransformToMine()
{
	// Play transform animation
	if (!TransformedForm || !UnitMovementComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABlaster::!TransformedForm || !UnitMovementComponent"));
		return;
	}
	
	Body->SetStaticMesh(TransformedForm);
	UnitMovementComponent->DestroyComponent();
	bIsAttacker = true;
	Strenght = 4.f;

	GetWorld()->GetTimerManager().SetTimer(AutodestructionHandle, this, &ABlaster::Autodestruction, AutodestructionTime, false);
}

void ABlaster::OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Explode(OtherActor);
}

void ABlaster::Explode(AActor* OtherActor)
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
void ABlaster::Autodestruction()
{
	KillMe();
}
