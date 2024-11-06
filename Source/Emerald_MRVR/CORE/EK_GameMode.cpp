#include "EK_GameMode.h"

#include "EngineUtils.h"
#include "Engine/TargetPoint.h"

void AEK_GameMode::GetAllTargetpoints()
{
	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		TargetPoints.Add(*It);
	}
	
}
