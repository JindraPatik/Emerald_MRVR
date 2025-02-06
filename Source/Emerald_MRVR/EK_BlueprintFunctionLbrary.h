#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EK_BlueprintFunctionLbrary.generated.h"

class USplineComponent;

UCLASS()
class EMERALD_MRVR_API UEK_BlueprintFunctionLbrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="Splines")
	USplineComponent* CreateSplinePath(
		UObject* WorldContextObject,
		FVector StartPoint,
		FVector EndPoint,
		TArray<AActor*> PathPoints,
		AActor* Outer);

	
};


