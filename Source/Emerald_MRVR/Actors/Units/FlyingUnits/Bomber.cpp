#include "Bomber.h"

#include "Emerald_MRVR/Actors/Projectiles/Projectile.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"

ABomber::ABomber(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABomber::BeginPlay()
{
	Super::BeginPlay();

}

void ABomber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FHitResult HitResult;
	if (!bFoundValidTarget)
	{
		PerformSphereTrace(HitResult);
	}
	
	if (AActor* HittedActor = HitResult.GetActor())
	{
		FindValidTarget(HittedActor);
	}
}

/* Sphere trace for searching AUnits */
void ABomber::PerformSphereTrace(FHitResult& OutHit) const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector(); // Osa X
	FQuat DownwardRotation = FQuat(RightVector, FMath::DegreesToRadians(60.f));
	FVector RotatedDirection = DownwardRotation.RotateVector(ForwardVector);
	FVector End = Start + RotatedDirection * 3000.f;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	FCollisionResponseParams CollisionResponseParams;
	CollisionResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	CollisionResponseParams.CollisionResponse.SetResponse(ECC_WorldDynamic,ECR_Block);
	CollisionResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	CollisionQueryParams.bDebugQuery = true;
	
	bool bHit = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(ScanningSphereRadius), CollisionQueryParams, CollisionResponseParams);

	// Debug Line trace
	/*FColor TraceColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(World, Start, End, TraceColor, false, 1.f);
	DrawDebugSphere(World, End, ScanningSphereRadius, 12, TraceColor, false, 1.f);*/
}

/* Checks if it is Enemy target */
void ABomber::FindValidTarget(AActor* Unit)
{
	AUnit* ValidClass = Cast<AUnit>(Unit); 
	if (ValidClass && GetOwner() != ValidClass->GetOwner())
	{
		StartBombingSequence();
		bFoundValidTarget = true;
	}
}

/* Timer for random spawning Projectiles in desired Interval */
void ABomber::StartBombingSequence()
{
	float Rate = FMath::RandRange(MinBombInterval, MaxBombInterval);
	GetWorld()->GetTimerManager().SetTimer(BombingSequence, this, &ABomber::SpawnProjectile, Rate, true);
}

/* Projectiles spawning */
void ABomber::SpawnProjectile()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = GetActorLocation() - FVector(0, 0 , 10.f) - GetActorForwardVector() * 3.f; // Offset
	FRotator Rotation = GetActorForwardVector().Rotation();
	
	AProjectile* ProjectileInst = World->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation, SpawnParameters);
	if (!ProjectileInst)
	{
		return;
	}
	
	ProjectileInst->Speed = Speed;
	ProjectilesCount++;
	
	if (ProjectilesCount >= ProjectilesToSpawn)
	{
		GetWorld()->GetTimerManager().ClearTimer(BombingSequence);
		// UnitMovementComponent->ReturnHome();
	}
}

