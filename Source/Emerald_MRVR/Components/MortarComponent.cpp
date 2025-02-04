#include "MortarComponent.h"

#include "CombatComponent.h"
#include "UnitMovementComponent.h"
#include "Emerald_MRVR/Unit.h"

UMortarComponent::UMortarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMortarComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
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
		MortarInstance->Strenght = 4.f;
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
	if (Unit)
	{
		if (Unit->GetOwner() != GetOwner()->GetOwner())
		{
			// Play Explosion
			if (Unit->Strenght < MineStrenght)
			{
				Unit->Destroy();
				GetOwner()->Destroy();
				UE_LOG(LogTemp, Log, TEXT("Mine Exploded"))
			}
		}
	}
}



