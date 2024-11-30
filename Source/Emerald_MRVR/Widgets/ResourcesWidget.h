// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourcesWidget.generated.h"

class UTextBlock;

UCLASS()
class EMERALD_MRVR_API UResourcesWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TXT_Resources;

	UFUNCTION(BlueprintCallable, Category="Widget")
	void UpdateResourcesWidget(float Resources);
	
};
