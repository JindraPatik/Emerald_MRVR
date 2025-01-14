#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThiefComponent.generated.h"


class AUnit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UThiefComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UThiefComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlapped(	UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);

	UFUNCTION()
		void HarvesterRobbery(AActor* OtherActor);

	UFUNCTION()
		void BaseRobbery(AActor* OtherActor);


	UPROPERTY()
		TObjectPtr<AUnit> Unit;
public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
		bool bIsloaded = false;

	UPROPERTY()
		float StealedValue = 0;

	UPROPERTY(EditDefaultsOnly, Category="Stealing")
		float AmountToStealFromBase = 10.f;
	
};
