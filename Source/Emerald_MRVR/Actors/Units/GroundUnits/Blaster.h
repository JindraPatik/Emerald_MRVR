#pragma once

#include "CoreMinimal.h"
#include "Emerald_MRVR/Actors/Units/Unit.h"
#include "Blaster.generated.h"

/* Attacking ground Unit which convetrs self to explosive mine which explodes when overlaps with enemy unit
 * Destroy self after a while when doesnt overlap with enemy unit.
 */
UCLASS()
class EMERALD_MRVR_API ABlaster : public AUnit
{
	GENERATED_BODY()

public:
	ABlaster(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Min Time from spawn to transformation to Mine */
	UPROPERTY(EditDefaultsOnly, Category="Behavior")
		float TransformationStartMin = 2.f;

	/* Max Time from spawn to transformation to Mine */
	UPROPERTY(EditDefaultsOnly, Category="Behavior")
		float TransformationStartMax = 4.f;

	/* Transformation to Mine Timer Function*/
	UFUNCTION()
		void BeginTransformation();

	/* Transformation to Mine form and starts autodestruction Timer */
	UFUNCTION()
		void TransformToMine();

	UFUNCTION()
	void OnActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

	
	UFUNCTION()
		void Explode(AActor* OtherActor);

	/* Destroy self after AutodestructionTime when it doesn't overlap with enemy unit */
	UFUNCTION()
		void Autodestruction();

	/* Time from transformation to Mine to Autodestruction */
	UPROPERTY(EditDefaultsOnly, Category="Behavior")
		float AutodestructionTime = 30.f;

	UPROPERTY()
		FTimerHandle AutodestructionHandle;

	UPROPERTY()
		bool bBlasterIsTransformed = false;

	UPROPERTY(EditDefaultsOnly, Category="Visuals")
		UStaticMesh* TransformedForm;
			
	FTimerHandle BeginTransformationHandle;

	/* Strenght of the Mine in Explosive form */
	UPROPERTY(EditDefaultsOnly, Category="Combat")
		float MineStrenght = 4;
};
