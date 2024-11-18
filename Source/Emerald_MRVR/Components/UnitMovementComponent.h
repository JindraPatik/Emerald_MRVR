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
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Spawning")
	void Server_SetTargetLoc();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="Spawning")
	void Multi_SetTargetLoc();
    
	UFUNCTION(Server, Unreliable)
	void Server_MoveTo(FVector TargetLocation) const;
    
	UFUNCTION(Server, Unreliable)
	void Multi_MoveTo(FVector TargetLocation) const;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="Moving", meta=(AllowPrivateAccess))
	FVector TargetLoc;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
