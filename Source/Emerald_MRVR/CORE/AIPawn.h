#pragma once

#include "CoreMinimal.h"
#include "VRPawn.h"
#include "GameFramework/Pawn.h"
#include "AIPawn.generated.h"

class UBehaviorTree;
class UMilitaryBaseComp;
class UHealthComponent;
class UResourcesComponent;
class AMilitaryBase;
class UAIComponent;
class AGameState;

UCLASS()
class EMERALD_MRVR_API AAIPawn : public APawn
{
	GENERATED_BODY()

public:
	AAIPawn();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void EndGame(APawn* Looser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
		TObjectPtr<UAIComponent> AI_Component;						//pb: pozor na konzistenci nazvu: AI_Component vs HealthComponent vs MilitaryBaseComp
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		UMilitaryBaseComp* MilitaryBaseComp;						//pb: probral bych duvody, proc je to Componenta - ja bych to udelal spise jako Actora, viz. doc Coding + Naming Standards

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UResourcesComponent> ResourcesComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Base")
		AMilitaryBase* BaseInstance;

	TObjectPtr<AGameState> AEK_GameStateInst;
		
	

};
