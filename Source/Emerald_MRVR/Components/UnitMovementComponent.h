#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitMovementComponent.generated.h"

class AMilitaryBase;
class APC_MR_General;
class AMR_General;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UUnitMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitMovementComponent();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement")
	bool bMovementEnabled = true;
	   
	UFUNCTION(BlueprintCallable, Category="Moving")
	void MoveTo(float DeltaTime) const;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="Stats")
	float UnitSpeed;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
