#include "MortarComponent.h"

#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"
#include "Emerald_MRVR/Actors/Unit.h"

UMortarComponent::UMortarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMortarComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	OwnerUnit = Cast<AUnit>(GetOwner());
	if (OwnerActor)
	{
		UPrimitiveComponent* Collisioncomp = Cast<UPrimitiveComponent>(OwnerActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		if (Collisioncomp)
		{
			Collisioncomp->OnComponentBeginOverlap.AddDynamic(this, &UMortarComponent::OnOwnerOverlap);
		}
	}
	
	
	BeginTransformation();
}

void UMortarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UMortarComponent::BeginTransformation()
{
	float TransformationStart = FMath::RandRange(TransformationStartMin, TransformationStartMax);
	GetWorld()->GetTimerManager().SetTimer(BeginTransformationHandle, this, &UMortarComponent::TransformToMine, TransformationStart, false);
}

void UMortarComponent::TransformToMine()
{
	// Play transform animation
	UUnitMovementComponent* MortarMovementComponent = GetOwner()->FindComponentByClass<UUnitMovementComponent>();
	AUnit* MortarInstance = Cast<AUnit>(GetOwner());
	if (TransformedSM && MortarInstance && MortarMovementComponent)
	{
		MortarInstance->Body->SetStaticMesh(TransformedSM);
		MortarMovementComponent->DestroyComponent();
		if (OwnerUnit)
		{
			OwnerUnit->bIsAttacker = true;
			MortarInstance->Strenght = 4.f;
		}

		GetWorld()->GetTimerManager().SetTimer(AutodestructionHandle, this, &UMortarComponent::Autodestruction, AutodestructionTime, false);
	}
}

void UMortarComponent::OnOwnerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode(OtherActor);
}

void UMortarComponent::Explode(AActor* OtherActor)
{
	AUnit* Unit = Cast<AUnit>(OtherActor);
	AUnit* Owner = Cast<AUnit>(GetOwner());
	if (Unit)
	{
		if (Unit->GetOwner() != GetOwner()->GetOwner())
		{
			// Play Explosion
			if (Unit->Strenght < MineStrenght)
			{
				Unit->KillMe();
				if (Owner)
				{
					Owner->KillMe();
				}
			}
		}
	}
}

void UMortarComponent::Autodestruction()
{
	AUnit* Owner = Cast<AUnit>(GetOwner());
	if (Owner)
	{
		Owner->KillMe();
	}
}



