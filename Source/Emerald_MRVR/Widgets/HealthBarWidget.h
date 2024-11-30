#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class EMERALD_MRVR_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TXT_Health;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;
		
};
