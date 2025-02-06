#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EK_FunctionLibrary.generated.h"

class USplineComponent;

UCLASS()
class EMERALD_MRVR_API UEK_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static USplineComponent* CreateSplinePath(
		UObject* WorldContextObject,
		FVector StartLocation,
		FVector EndLocation,
		bool bIsReversed, TSubclassOf<AActor> PathPointClass);

};
