#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIPawn.generated.h"

class UMilitaryBaseComp;
class UHealthComponent;
class UResourcesComponent;
class AMilitaryBase;

UCLASS()
class EMERALD_MRVR_API AAIPawn : public APawn
{
	GENERATED_BODY()

public:
	AAIPawn();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Body")
		UMilitaryBaseComp* MilitaryBaseComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UResourcesComponent> ResourcesComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Base")
		AMilitaryBase* BaseInstance;
	

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
