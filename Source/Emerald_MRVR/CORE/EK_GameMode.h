#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EK_GameMode.generated.h"

class APC_MR_General;
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
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;
	virtual void Logout(AController* Exiting) override;
	
	UPROPERTY(BlueprintReadOnly, Category="Networkong")
	TArray<APlayerController*> AllPCs;
	 
	UFUNCTION(Category="Spawning")
	FTransform FindMyPlayerStart();

	UFUNCTION(Category="Spawning")
	void SpawnPlayer(APlayerController* PlayerController);

	UFUNCTION()
	void FindAllPlayerStarts();
	
	UFUNCTION(BlueprintCallable)
	TArray<ATargetPoint*> GetAllTargetpoints();

	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Spawning")
	TArray<ATargetPoint*> TargetPoints;

	UPROPERTY(BlueprintReadOnly, Category="Spawning")
	TArray<APlayerStart*> AllPlayerStarts;

	UPROPERTY(BlueprintReadOnly, Category="Spawning")
	AMR_General* PlayerPawn;

	UPROPERTY(EditAnywhere)
	UCrystalSpawnerComp* CrystalSpawner;

	UPROPERTY(EditDefaultsOnly, Category="Identification")
	TArray<UMaterialInterface*> PlayersColors;




};
