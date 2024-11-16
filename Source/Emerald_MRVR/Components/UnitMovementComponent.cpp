#include "UnitMovementComponent.h"

#include "Emerald_MRVR/DebugMacros.h"
#include "Emerald_MRVR/MilitaryBase.h"
#include "Emerald_MRVR/CORE/MR_General.h"
#include "Emerald_MRVR/CORE/PC_MR_General.h"


UUnitMovementComponent::UUnitMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UUnitMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	MilitaryBase = Cast<AMilitaryBase>(GetOwner());
	PC = Cast<APC_MR_General>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController());
	
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetTargetLoc();
}

void UUnitMovementComponent::SetTargetLoc()
{
	if (MilitaryBase) // <---- tohle není platné!
	{
		// Zjistit target loc ze základny která není moje
		// TargetLoc = MilitaryBase->GetSpawnpointGround().GetLocation();
		TArray<ULocalPlayer*> LocalPlayers = GetWorld()->GetGameInstance()->GetLocalPlayers();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(3, 10.f,FColor::Purple, FString::Printf(TEXT("Local players: %d"), LocalPlayers.Num()));
		}
	}
	D5B("NotMilitaryBase"); 
}
