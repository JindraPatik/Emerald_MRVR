#include "UnitMovementComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/Unit.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/CORE/PC_MR_General.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UUnitMovementComponent::UUnitMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bAutoActivate = true;
	SetIsReplicated(true);
}

void UUnitMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUnitMovementComponent, UnitSpeed);
}

void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AUnit* Unit = Cast<AUnit>(GetOwner());
	UnitSpeed = Unit->Speed;
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UUnitMovementComponent::MoveTo(float DeltaTime) const
{
	AUnit* Unit = Cast<AUnit>(GetOwner());
	if (!Unit)
	{
		return;
	}

	if (bMovementEnabled)
	{
		FVector ForwardDirection = Unit->GetActorForwardVector(); // Směr vpřed
		ForwardDirection.Normalize();

		// Konstantní rychlost vpřed
		FVector ForwardMovement = ForwardDirection * (UnitSpeed * DeltaTime);

		// Oscilace do stran a nahoru/dolů
		UWorld* World = GetWorld();
		if (World)
		{
			float Time = GetWorld()->GetTimeSeconds(); // Aktuální čas ve hře
			float SidewaysOscillation = FMath::Sin(Time * OscillationFrequency) * OscillationAmplitudeX; // Oscilace do stran
			float VerticalOscillation = FMath::Sin(Time * OscillationFrequency * 0.5f) * OscillationAmplitudeZ; // Oscilace nahoru/dolů

			FVector OscillationOffset = FVector(SidewaysOscillation, 0.f, VerticalOscillation);

			// Spočítání celkového pohybu
			FVector DeltaLocation = ForwardMovement + OscillationOffset;
			Unit->AddActorWorldOffset(DeltaLocation);

			// Simulace Roll na základě pohybu do stran
			float TargetRoll = SidewaysOscillation * RollFactor;
			FRotator CurrentRotation = Unit->GetActorRotation();
			FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, TargetRoll);

			// Nastavení nové rotace
			Unit->SetActorRotation(NewRotation);
		}
	}
}

void UUnitMovementComponent::Turn180()
{
	FRotator BackwardRotation = GetOwner()->GetActorRotation() + FRotator(0, 180, 0);
	GetOwner()->SetActorRotation(BackwardRotation);
}

