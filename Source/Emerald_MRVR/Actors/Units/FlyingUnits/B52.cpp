#include "B52.h"

#include "Emerald_MRVR/Actors/Projectiles/Projectile.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"

AB52::AB52()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AB52::BeginPlay()
{
	Super::BeginPlay();
}

void AB52::Tick(float DeltaTime)
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
void AB52::PerformSphereTrace(FHitResult& OutHit) const
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
	FColor TraceColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(World, Start, End, TraceColor, false, 1.f);
	DrawDebugSphere(World, End, ScanningSphereRadius, 12, TraceColor, false, 1.f);
}

/* Checks if it is Enemy target */
void AB52::FindValidTarget(AActor* Unit)
{
	AUnit* ValidClass = Cast<AUnit>(Unit); 
	if (ValidClass && GetOwner() != ValidClass->GetOwner())
	{
		StartBombingSequence();
		bFoundValidTarget = true;
	}
}

/* Timer for random spawning Projectiles in desired Interval */
void AB52::StartBombingSequence()
{
	float Rate = FMath::RandRange(MinBombInterval, MaxBombInterval);
	GetWorld()->GetTimerManager().SetTimer(BombingSequence, this, &AB52::SpawnProjectile, Rate, true);
}

/* Projectiles spawning */
void AB52::SpawnProjectile()
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

