#include "B52Component.h"

#include "Emerald_MRVR/Unit.h"


UB52Component::UB52Component()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UB52Component::BeginPlay()
{
	Super::BeginPlay();
	
}


void UB52Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FHitResult HitResult;
	PerformSphereTrace(HitResult);
	AActor* HittedActor = HitResult.GetActor();
	if (HittedActor)
	{
		FindValidTarget(HittedActor);
	}

}

void UB52Component::PerformSphereTrace(FHitResult& OutHit)
{
	UWorld* World = GetWorld();
	if (World && GetOwner() && !bFoundValidTarget)
	{
		FVector Start = GetOwner()->GetActorLocation();
		FVector ForwardVector = GetOwner()->GetActorLocation().ForwardVector;
		FRotator Downwardrotation = FRotator(-60.f, -90.f,0.f);
		FVector RotatedDirection = Downwardrotation.RotateVector(ForwardVector);
		FVector End = Start + RotatedDirection * 30000.f;
		float SphereRadius = 100.f;
		
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bDebugQuery = true;
		
		bool bHit = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeSphere(SphereRadius), CollisionQueryParams);
		
	/*FColor TraceColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(World, Start, End, TraceColor, false, 1.f);
	DrawDebugSphere(World, End, SphereRadius, 12, TraceColor, false, 1.f);*/
	}
}

void UB52Component::FindValidTarget(AActor* Unit)
{
	AUnit* ValidClass = Cast<AUnit>(Unit);
	UB52Component* ValidUB52Component = Unit->FindComponentByClass<UB52Component>();
	if (ValidClass && ValidUB52Component && !bFoundValidTarget)
	{
		bFoundValidTarget = true;
		SpawnProjectile();
	}
}

void UB52Component::SpawnProjectile()
{
	
}

