#include "GM.h"
#include "TimerManager.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"
#include "Emerald_MRVR/DebugMacros.h"
#include "Engine/World.h"


void AGM::BeginPlay()
{
	Super::BeginPlay();
}

void AGM::StartCountdown()
{
	SpawnCountDownWidgetActor();
	GetWorld()->GetTimerManager().SetTimer(CountDownHandle, this, &AGM::DecreaseCounter, 1.0f, true);
}

void AGM::DecreaseCounter()
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

void AGM::SpawnCountDownWidgetActor()
{
	DBG(5.f, "GM::CD widget spawned")
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector Location = FVector(0.f, 0.f, 200.f);
	FRotator Rotation = FRotator::ZeroRotator;
	CountDownWidgetActorInstance = GetWorld()->SpawnActor<AActor>(CountDownWidgetActor, Location, Rotation, SpawnParameters);
}

void AGM::StartGame()
{
	bGameHasStarted = true;
}

void AGM::EndGame(APawn* Looser)
{
	bGameHasEnded = true;
	OnGameEndedDelegate.Broadcast(Looser);
}




