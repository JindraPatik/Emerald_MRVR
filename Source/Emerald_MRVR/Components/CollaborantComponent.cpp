#include "CollaborantComponent.h"

#include "BoxComponent.h"
#include "UnitMovementComponent.h"
#include "Emerald_MRVR/Unit.h"

UCollaborantComponent::UCollaborantComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCollaborantComponent::BeginPlay()
{
	Super::BeginPlay();

	AUnit* Unit = Cast<AUnit>(GetOwner());
	if (Unit)
	{
		Unit->BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UCollaborantComponent::OnBoxOverlapped);
	}
	
}

void UCollaborantComponent::KillMe()
{
	GetOwner()->Destroy();
}

void UCollaborantComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCollaborantComponent::OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUnit* OtherUnit = Cast<AUnit>(OtherActor);
	if (OtherUnit && OtherUnit->GetOwner() != GetOwner()->GetOwner())
	{
		OtherActor->Owner = GetOwner()->GetOwner();
		UUnitMovementComponent* OtherUnitMovementComp = OtherActor->FindComponentByClass<UUnitMovementComponent>();
		if (OtherUnitMovementComp)
		{
			OtherUnitMovementComp->Turn180();
		}

		UUnitMovementComponent* MyMovementComponent = GetOwner()->FindComponentByClass<UUnitMovementComponent>();
		if (MyMovementComponent)
		{
			MyMovementComponent->TurnRandom();
			MyMovementComponent->UnitSpeed = MyMovementComponent->UnitSpeed + ((MyMovementComponent->UnitSpeed / 100) * 40.f);
			FTimerHandle KillMeHandle;
			GetWorld()->GetTimerManager().SetTimer(KillMeHandle, this, &UCollaborantComponent::KillMe, 15.f, false);
		}
	}
}

