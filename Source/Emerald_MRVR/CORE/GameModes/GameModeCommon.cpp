#include "GameModeCommon.h"
#include "TimerManager.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/Support/DebugMacros.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Emerald_MRVR/Components/Unit/Movement/UnitMovementComponent.h"
#include "Emerald_MRVR/CORE/EKGameState.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


void AGameModeCommon::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	GameState = World->GetGameState<AEKGameState>();
}

void AGameModeCommon::StartCountdown()
{
	SpawnCountDownWidgetActor();
	GetWorld()->GetTimerManager().SetTimer(CountDownHandle, this, &AGameModeCommon::DecreaseCounter, 1.0f, true);
}

void AGameModeCommon::DecreaseCounter()
{
	UWidgetComponent* CountdownWidgetInst = CountDownWidgetActorInstance->FindComponentByClass<UWidgetComponent>();
	UTextBlock* TXT_CountDown = Cast<UTextBlock>(CountdownWidgetInst->GetWidget()->WidgetTree->FindWidget("TXT_CountDown"));
	if (CountDownValue > 0)
	{
		CountDownValue--;
		if (CountdownWidgetInst && TXT_CountDown)
		{
			TXT_CountDown->SetText(FText::AsNumber(CountDownValue));
		}
	}
	else if (CountDownValue == 0)
	{
		if (CountdownWidgetInst && TXT_CountDown)
		{
			TXT_CountDown->SetText(FText::FromString("Start"));
		}
		
		OnGameStartedDelegate.Broadcast();
		StartGame();
		GetWorld()->GetTimerManager().ClearTimer(CountDownHandle);
		CountDownWidgetActorInstance->Destroy();
	}
}

void AGameModeCommon::SpawnCountDownWidgetActor()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = FVector(0.f, 0.f, 200.f);
	FRotator Rotation = FRotator::ZeroRotator;
	CountDownWidgetActorInstance = GetWorld()->SpawnActor<AActor>(CountDownWidgetActor, Location, Rotation, SpawnParameters);
}


void AGameModeCommon::StartGame()
{
	if (!GameState)
	{
		return;
	}
	GameState->CurrentMatchState = E_MatchState::E_Started;
}

void AGameModeCommon::EndGame(APawn* InPawn)
{
	GameState->CurrentMatchState = E_MatchState::E_Ended;
	StopAllUnits();
	OnGameEndedDelegate.Broadcast(InPawn);
	
}

void AGameModeCommon::StopAllUnits()
{
	TArray<AActor*> AllUnits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnit::StaticClass(), AllUnits);

	for (AActor* Unit : AllUnits)
	{
		AUnit* MovingUnit = Cast<AUnit>(Unit);
		if (!MovingUnit)
		{
			return;
		}
		
		UUnitMovementComponent* MovementComponent = MovingUnit->FindComponentByClass<UUnitMovementComponent>();
		if (!MovementComponent)
		{
			return;
		}
		MovementComponent->bMovementEnabled = false;
	}
}



