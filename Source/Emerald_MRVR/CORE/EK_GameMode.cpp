#include "EK_GameMode.h"

#include "EngineUtils.h"
#include "Engine/TargetPoint.h"

TArray<ATargetPoint*> AEK_GameMode::GetAllTargetpoints()
{
	TArray<ATargetPoint*> TargetPoints;
	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		TargetPoints.Add(*It);
	}
	return TargetPoints;
}
