#include "GM.h"
#include "TimerManager.h"
#include "Engine/World.h"

void AGM::StartCountdown()
{
	GetWorld()->GetTimerManager().SetTimer(CountDownHandle, this, &AGM::DecreaseCounter, 1.0f, true);
}

void AGM::DecreaseCounter()
{
	if (CountDownValue > 0)
	{
		CountDownValue--;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%d"), CountDownValue));
	}
	else if (CountDownValue == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("MATCH STARTED MATCH STARTED MATCH STARTED MATCH STARTED MATCH STARTED ")));
		UE_LOG(LogTemp, Log, TEXT("MATCH STARTED MATCH STARTED MATCH STARTED MATCH STARTED MATCH STARTED"));
		GetWorld()->GetTimerManager().ClearTimer(CountDownHandle);
		StartMatch();
		return;
	}
}


