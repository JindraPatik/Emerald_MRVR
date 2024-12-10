#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


class UHealthBarWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();
	

	UPROPERTY(ReplicatedUsing=OnRep_OnHealthChanged, EditDefaultsOnly, BlueprintReadWrite, Category="Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
	float MaxHealth;

	UFUNCTION()
	void OnRep_OnHealthChanged();

	UPROPERTY(BlueprintReadWrite, Category="Widget")
	UHealthBarWidget* HealthWidget;

	

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
