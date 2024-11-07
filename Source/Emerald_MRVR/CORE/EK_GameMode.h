#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EK_GameMode.generated.h"

class ATargetPoint;
class APlayerStart;
/**
 * 
 */
UCLASS()
class EMERALD_MRVR_API AEK_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AEK_GameMode();
	void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	
	UFUNCTION(BlueprintCallable)
	TArray<ATargetPoint*> GetAllTargetpoints();

	TArray<ATargetPoint*> TargetPoints;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<APlayerStart*> PlayerStarts;
};
