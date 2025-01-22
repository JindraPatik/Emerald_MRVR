#include "RocketComponent.h"

#include "BoxComponent.h"
#include "EngineUtils.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/Unit.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

URocketComponent::URocketComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URocketComponent::BeginPlay()
{
	Super::BeginPlay();
	AUnit* Unit = Cast<AUnit>(GetOwner());
	if (Unit)
	{
		Unit->BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &URocketComponent::OnBoxOverlapped);
	}
	CurrentSpeed = 0.0f; // Startovní rychlost
	CurrentDirection = GetOwner()->GetActorForwardVector().GetSafeNormal(); // Výchozí směr letu
	SearchTargets();
	GetWorld()->GetTimerManager().SetTimer(LaunchHandle, this, &URocketComponent::RocketStarted, LaunchTime, false);
	Missed();
}

void URocketComponent::OnBoxOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		KillTarget(OtherActor);
		KillMe();
	}
}

void URocketComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()) return;

	!bRocketHasLaunched ? LaunchRocket(DeltaTime) : MoveToTarget(DeltaTime);

	if (bHasTargeted && !Target)
	{
		PreviousTarget = nullptr;
		KillMe();
	}
}

void URocketComponent::FindTarget()
{
	UWorld* World = GetWorld();
	if (!World) return;
	ListOfTargets.Empty();

	for (TActorIterator<AUnit> It(World); It; ++It)
	{
		AUnit* Unit = *It;
		if (Unit)
		{
			if (Unit->GetOwner() != GetOwner()->GetOwner() && Unit->bIsFlyingUnit)
			{
				ListOfTargets.Add(Unit);
			}
		}
	}
}

void URocketComponent::SelectTarget()
{
	if (ListOfTargets.Num() > 0)
	{
		int ItemsInArray = ListOfTargets.Num() - 1;
		int Index = FMath::RandRange(0, ItemsInArray);
		Target = ListOfTargets[Index];
		if (Target)
		{
			bHasTargeted = true;
		}
	}
}

void URocketComponent::LaunchRocket(float DeltaTime)
{
	AActor* Owner = GetOwner();
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, MaxLaunchSpeed, DeltaTime, StartAccelerationRate);
	FVector DeltaLocation = CurrentDirection * CurrentSpeed * DeltaTime;
	Owner->AddActorWorldOffset(DeltaLocation);
}

void URocketComponent::RocketStarted()
{
	bRocketHasLaunched = true;
}

void URocketComponent::MoveToTarget(float DeltaTime)
{
	/*
	AActor* Owner = GetOwner();

	// Zrychlení
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, MaxSpeed, DeltaTime, AccelerationRate);

	// Pokud je Target platný, nastav směr k němu
	if (Target)
	{
		FVector TargetLocation = Target->GetActorLocation();
		FVector CurrentLocation = Owner->GetActorLocation();
		PreviousTarget = Target;

		// Aktualizace směru k cíli
		CurrentDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
	}

	// Pohyb ve směru `CurrentDirection
	FVector DeltaLocation = CurrentDirection * CurrentSpeed * DeltaTime;
	Owner->AddActorWorldOffset(DeltaLocation); // Posun s kolizí (true)

	// Plynulá rotace směrem k cíli
	FRotator CurrentRotation = Owner->GetActorRotation();
	FRotator TargetRotation = CurrentDirection.Rotation();
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

	Owner->SetActorRotation(SmoothRotation);
	*/

	AActor* Owner = GetOwner();
	if (!Owner) return;

	// Zrychlení
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, MaxSpeed, DeltaTime, AccelerationRate);

	// Pokud je Target platný, nastav směr k němu
	if (Target && !bLaunched)
	{
		FVector TargetLocation = Target->GetActorLocation();
		FVector CurrentLocation = Owner->GetActorLocation();

		// Vypočítat počáteční směr s ohledem na oblouk (přidání vertikální složky)
		FVector LaunchDirection = FVector(0, 0, 1); // Start kolmo vzhůru
		CurrentVelocity = LaunchDirection * CurrentSpeed;

		bLaunched = true; // Raketa byla vystřelena
	}

	// Pokud je Target platný, postupně zaměřuj na něj
	if (Target)
	{
		FVector TargetLocation = Target->GetActorLocation();
		FVector CurrentLocation = Owner->GetActorLocation();

		FVector TargetDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
		CurrentDirection = FMath::VInterpTo(CurrentDirection, TargetDirection, DeltaTime, DirectionChangeSpeed);
	}

	// Přidání gravitace do aktuální rychlosti
	CurrentVelocity.Z -= Gravity * DeltaTime;

	// Pohyb rakety na základě aktuální rychlosti a směru
	FVector DeltaLocation = CurrentDirection * CurrentSpeed * DeltaTime;
	Owner->AddActorWorldOffset(DeltaLocation, true); // Pohyb s kolizí

	// Plynulá rotace ve směru pohybu
	if (!CurrentVelocity.IsNearlyZero())
	{
		FRotator CurrentRotation = Owner->GetActorRotation();
		FRotator TargetRotation = CurrentDirection.Rotation();
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
		Owner->SetActorRotation(SmoothRotation);
	}
}

void URocketComponent::SearchTargets()
{
	if (!Target)
	{
		GetWorld()->GetTimerManager().SetTimer(SearchHandle, this, &URocketComponent::FindAndSelect, 0.5f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SearchHandle);
	}
}

void URocketComponent::FindAndSelect()
{
	FindTarget();
	SelectTarget();
}

void URocketComponent::KillMe()
{
	AUnit* Ownerunit = Cast<AUnit>(GetOwner());
	if (Ownerunit)
	{
		Ownerunit->BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Ownerunit->Body->SetVisibility(false);
		DestroyMe();

		/*if (Explosion)
		{
			UNiagaraComponent* ExplosionEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Explosion, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, FVector(0.001,0.001,0.001), true);
			DestroyMe();
		}*/
		
		//GetWorld()->GetTimerManager().SetTimer(DestroyHandle, this, &URocketComponent::DestroyMe, 2.f, false);
	}

}

void URocketComponent::DestroyMe()
{
	GetOwner()->Destroy();
}

void URocketComponent::Missed()
{
	GetWorld()->GetTimerManager().SetTimer(MissedHandle, this, &URocketComponent::KillMe, Lifetime, false);
}

void URocketComponent::KillTarget(AActor* TargetActor)
{
	if (TargetActor)
	{
		AUnit* TargetEnemy = Cast<AUnit>(TargetActor);
		if (TargetEnemy && TargetEnemy->GetOwner() && TargetEnemy->GetOwner() != GetOwner()->GetOwner())
		{
			TargetEnemy->Destroy();
		}
	}
}

