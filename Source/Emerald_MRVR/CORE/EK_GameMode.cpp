#include "EK_GameMode.h"

#include "EngineUtils.h"
#include "MR_General.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/PlayerStart.h"

AEK_GameMode::AEK_GameMode()
{
	
}

void AEK_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AEK_GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TargetPoints = GetAllTargetpoints();
}

TArray<ATargetPoint*> AEK_GameMode::GetAllTargetpoints()
{
	TArray<ATargetPoint*> TPs;
	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		TargetPoints.Add(*It);
	}
	return TPs;
}




