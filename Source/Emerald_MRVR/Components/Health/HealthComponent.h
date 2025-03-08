#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthBarWidget;
class AGameModeMultiplayer;
class UMilitaryStationComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent(const FObjectInitializer& ObjectInitializer);

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
		TObjectPtr<UMilitaryStationComponent> MilitaryBaseCompInstance;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
