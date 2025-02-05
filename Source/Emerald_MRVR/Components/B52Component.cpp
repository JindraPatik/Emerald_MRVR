#include "B52Component.h"

#include "UnitMovementComponent.h"
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
		FVector ForwardVector = GetOwner()->GetActorForwardVector();
		FVector RightVector = GetOwner()->GetActorRightVector();

		FQuat DownwardRotation = FQuat(RightVector, FMath::DegreesToRadians(60.f));
		FVector RotatedDirection = DownwardRotation.RotateVector(ForwardVector);

		FVector End = Start + RotatedDirection * 30000.f;
		
		float SphereRadius = 5.f;
		
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetOwner());
		FCollisionResponseParams CollisionResponseParams;
		CollisionResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
		CollisionResponseParams.CollisionResponse.SetResponse(ECC_WorldDynamic,ECR_Block);
		CollisionResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
		CollisionQueryParams.bDebugQuery = true;
		
		bool bHit = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereRadius), CollisionQueryParams, CollisionResponseParams);

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
		StartBombingSequence();
		bFoundValidTarget = true;
	}
}

void UB52Component::StartBombingSequence()
{
	float Rate = FMath::RandRange(0.5f, MaxBombInterval);
	GetWorld()->GetTimerManager().SetTimer(BombingSequence, this, &UB52Component::SpawnProjectile, Rate, true);
}

void UB52Component::SpawnProjectile()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector Location = GetOwner()->GetActorLocation() - FVector(0, 0 , 10.f) - GetOwner()->GetActorForwardVector() * 3.f; // Offset
		FRotator Rotation = GetOwner()->GetActorForwardVector().Rotation();
		
		AProjectile* ProjectileInst = World->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation, SpawnParameters);
		if (ProjectileInst)
		{
			AUnit* OwnerUnit = Cast<AUnit>(GetOwner());
			if (OwnerUnit)
			{
				ProjectileInst->Speed = OwnerUnit->Speed;
				BombsCount++;
				if (BombsCount > 2)
				{
					GetWorld()->GetTimerManager().ClearTimer(BombingSequence);
					UUnitMovementComponent* MovementComponent = GetOwner()->FindComponentByClass<UUnitMovementComponent>();
					if (MovementComponent)
					{
						MovementComponent->TurnRandom();
					}
				}
			}
		}
	}
}

