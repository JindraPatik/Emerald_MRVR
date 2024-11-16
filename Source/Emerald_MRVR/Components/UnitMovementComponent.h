#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitMovementComponent.generated.h"

class AMilitaryBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UUnitMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitMovementComponent();
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Moving", meta=(AllowPrivateAccess))
	FVector TargetLoc;
	
	AMilitaryBase* MilitaryBase;

	void SetTargetLoc();
	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
