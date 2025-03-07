#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CommonPawn.generated.h"

class APowerUp;

UCLASS()
class EMERALD_MRVR_API ACommonPawn : public APawn
{
	GENERATED_BODY()

public:
	ACommonPawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PowerUp")
		TArray<APowerUp*> AvailablePowerUps;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="PowerUp")
		TObjectPtr<APowerUp> SelectedPowerUp;

	UFUNCTION(BlueprintCallable, Category="PowerUp")
	void CyclePowerUps();

	UPROPERTY(BlueprintReadWrite, Category="PowerUp")
	int32 PowerUpIndex = 0;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		void AddPowerUp(APowerUp* InPowerUp);

	UFUNCTION(BlueprintCallable, Category="PowerUp")
		void ActivatePowerUp();
};
