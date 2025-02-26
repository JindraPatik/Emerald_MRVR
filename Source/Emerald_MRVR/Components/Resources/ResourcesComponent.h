#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourcesComponent.generated.h"

class UResourcesWidget;
class AGameModeMultiplayer;
class UMilitaryStationComp;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMERALD_MRVR_API UResourcesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UResourcesComponent();

	UPROPERTY(ReplicatedUsing=OnRep_ResourcesChanged, EditAnywhere, BlueprintReadWrite, Category="Resources")
		float AvailableResources;

	UFUNCTION(Blueprintable, Category="Resources")
		void UpdateResources(float ResourcesDelta);

	UFUNCTION(Server, Reliable)
		void Server_UpdateResources(float ResourcesDelta);

	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
		TObjectPtr<UMilitaryStationComp> MilitaryBaseCompInst;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Resources", meta=(AllowPrivateAccess="true"));
		float MaxResources = 9999.f;

	UFUNCTION()
		void OnRep_ResourcesChanged() const;

protected:
	void GrowResources();

public:	
	void StartGrowResources();

	UFUNCTION()
		void StopGrowResources(APawn* Looser);

		
	
};
