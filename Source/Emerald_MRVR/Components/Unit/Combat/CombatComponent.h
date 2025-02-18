#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"
enum class ECombatScenarios;
class AUnit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	
	UFUNCTION()
	void OnBoxOverlapped(	UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);

	UFUNCTION()
		void UnitFight(AActor* InActor);

	UFUNCTION()
		void BaseFight(AActor* InActor);

	FTimerHandle FightSequenceHandle;

	ECombatScenarios CurrentScenario;


protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		AUnit* Unit;
};
