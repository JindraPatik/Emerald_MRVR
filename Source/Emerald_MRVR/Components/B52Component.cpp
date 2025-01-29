#include "B52Component.h"

#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/Projectile.h"
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
	if (!bFoundValidTarget)
	{
		PerformSphereTrace(HitResult);
	}
	
	AActor* HittedActor = HitResult.GetActor();
	if (HittedActor)
	{
		FindValidTarget(HittedActor);
	}

}

void UB52Component::PerformSphereTrace(FHitResult& OutHit)
{
	UWorld* World = GetWorld();
	if (World && GetOwner())
	{
		FVector Start = GetOwner()->GetActorLocation();
		FVector ForwardVector = GetOwner()->GetActorLocation().ForwardVector;
		FRotator Downwardrotation = FRotator(-60.f, -90.f,0.f);
		FVector RotatedDirection = Downwardrotation.RotateVector(ForwardVector);
		FVector End = Start + RotatedDirection * 30000.f;
		float SphereRadius = 10.f;
		
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bDebugQuery = true;
		
		bool bHit = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius), CollisionQueryParams);
		
	FColor TraceColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(World, Start, End, TraceColor, false, 1.f);
	DrawDebugSphere(World, End, SphereRadius, 12, TraceColor, false, 1.f);
	}
}

void UB52Component::FindValidTarget(AActor* Unit)
{
	AUnit* ValidClass = Cast<AUnit>(Unit); // toto musí být empty
	if (ValidClass && GetOwner()->GetOwner() != ValidClass->GetOwner())
	{
		SpawnProjectile();
		bFoundValidTarget = true;
	}
}

void UB52Component::SpawnProjectile()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector Location = GetOwner()->GetActorLocation();
		FRotator Rotation = FRotator::ZeroRotator;
		
		World->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation, SpawnParameters);
		DBG(5, "Spawned bomb");
	}
}

