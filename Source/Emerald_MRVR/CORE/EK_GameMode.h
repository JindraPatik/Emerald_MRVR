#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EK_GameMode.generated.h"

class ATargetPoint;
class APlayerStart;
class UCrystalSpawnerComp;
class AMR_General;

UCLASS()
class EMERALD_MRVR_API AEK_GameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Pawn")
	TSubclassOf<AMR_General> PawnToSpawn;
public:
	AEK_GameMode();
	void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<ATargetPoint*> GetAllTargetpoints();

	UFUNCTION()
	void GetAllPlayerStarts();
	
	UPROPERTY(Replicated)
	TArray<ATargetPoint*> TargetPoints;

	UPROPERTY(EditAnywhere)
	UCrystalSpawnerComp* CrystalSpawner;

	UPROPERTY()
	TArray<APlayerStart*> PlayerStarts;
};
