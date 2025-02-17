#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthBarWidget;
class AMultiplayer_GameMode;
class UMilitaryBaseComp;


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

private:
	UPROPERTY()
		TObjectPtr<UMilitaryBaseComp> MilitaryBaseCompInst;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
