#pragma once

#include "CoreMinimal.h"
#include "CommonPawn.h"
#include "VRPawn.h"
#include "GameFramework/Pawn.h"
#include "AIPawn.generated.h"

class UBehaviorTree;
class UMilitaryStationComponent;
class UHealthComponent;
class UResourcesComponent;
class AMilitaryStation;
class UAIComponent;
class AGameState;

UCLASS()
class EMERALD_MRVR_API AAIPawn : public ACommonPawn
{
	GENERATED_BODY()

public:
	AAIPawn(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
		void StartGame();

	UFUNCTION()
		void EndGame(APawn* Looser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
		TObjectPtr<UAIComponent> AIComponent;						//pb: pozor na konzistenci nazvu: AI_Component vs HealthComponent vs MilitaryBaseComp
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		UMilitaryStationComponent* MilitaryStationComponent;						//pb: probral bych duvody, proc je to Componenta - ja bych to udelal spise jako Actora, viz. doc Coding + Naming Standards

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UResourcesComponent> ResourcesComponent;
	
	TObjectPtr<AGameState> GameStateInstance;

};
