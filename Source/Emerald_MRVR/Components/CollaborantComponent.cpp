#include "CollaborantComponent.h"

#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Emerald_MRVR/Actors/Units/GroundUnits/Mortar.h"

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
		Unit->Body->OnComponentBeginOverlap.AddDynamic(this, &UCollaborantComponent::OnBoxOverlapped);
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
		if (AMortar* MortarUnit = Cast<AMortar>(OtherUnit))
		{
			if (MortarUnit->bMortarIsTransformed)
			{
				MortarUnit->Explode(GetOwner());
				return;
			}
		}
		
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

