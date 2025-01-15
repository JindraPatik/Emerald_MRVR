#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollaborantComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UCollaborantComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCollaborantComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void KillMe();

	UFUNCTION()
	void OnBoxOverlapped(	UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex,
							bool bFromSweep,
							const FHitResult& SweepResult);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
