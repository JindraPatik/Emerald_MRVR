#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI_Component.generated.h"


class AGameState;
class ACrystal;
class AEKGameState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UAI_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAI_Component();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	TObjectPtr<AGameState> AEK_GameStateInst;

	UFUNCTION()
	void OnCrystalOccured(FVector CrystalLoc, ACrystal* CrystalInst);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
