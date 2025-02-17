#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EmeraldBlueprintFunctionLibrary.generated.h"

class APathPoint;
class USplineComponent;

UCLASS()
class EMERALD_MRVR_API UEmeraldBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="Splines")
	static USplineComponent* CreateSplinePath(
		UObject* WorldContextObject,
		FVector StartPoint,
		FVector EndPoint,
		TArray<APathPoint*> PathPoints,
		AActor* Outer);

	UFUNCTION(BlueprintCallable, Category="Splines")
	static TArray<APathPoint*> SortPathPoints(
			UObject* WorldContextObject,
			TSubclassOf<APathPoint> PathPointClass,
			bool bIsReversed);

	
};


