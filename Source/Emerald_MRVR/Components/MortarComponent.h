#pragma once

#include "CoreMinimal.h"
#include "ActorComponent.h"
#include "MortarComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EMERALD_MRVR_API UMortarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMortarComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Behavior")
		float TransformationStartMin = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Behavior")
		float TransformationStartMax = 4.f;
	
	UFUNCTION(BlueprintCallable, Category="Behavior")
		void BeginTransformation();

	UFUNCTION(BlueprintCallable, Category="Behavior")
		void TransformToMine();

	UFUNCTION()
	void OnOwnerOverlap(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor, 
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, Category="Behavior")
		void Explode(AActor* OtherActor);

	UPROPERTY(BlueprintReadWrite, Category="Behavior")
		bool bMortarIsTransformed = false;

	UPROPERTY(EditDefaultsOnly, Category="Visuals")
		UStaticMesh* TransformedSM;
			
	UPROPERTY()
		FTimerHandle BeginTransformationHandle;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
		float MineStrenght = 4;

	
};
