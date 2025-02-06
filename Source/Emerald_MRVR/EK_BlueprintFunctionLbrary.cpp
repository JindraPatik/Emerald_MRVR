#include "EK_BlueprintFunctionLbrary.h"
#include "SplineComponent.h"

USplineComponent* UEK_BlueprintFunctionLbrary::CreateSplinePath(UObject* WorldContextObject, FVector StartPoint,
                                                                FVector EndPoint, TArray<AActor*> PathPoints, AActor* Outer)
{
	if (!WorldContextObject) return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;

	USplineComponent* SplineComponent = NewObject<USplineComponent>(Outer, USplineComponent::StaticClass());
	if (!SplineComponent) return nullptr;

	SplineComponent->RegisterComponent();

	SplineComponent->ClearSplinePoints();
	SplineComponent->AddSplinePointAtIndex(StartPoint, 0, ESplineCoordinateSpace::World); // Adds Spline Starting point

	if (!PathPoints.IsEmpty()) // Adds Spline middle points
	{
		int32 Index = 1;
		for (AActor* PathPoint : PathPoints)
		{
			SplineComponent->AddSplinePointAtIndex(PathPoint->GetActorLocation(), Index, ESplineCoordinateSpace::World);
			Index++;
		}
	}

	int32 LastIndex = SplineComponent->GetNumberOfSplinePoints() - 1;
	SplineComponent->AddSplinePointAtIndex(EndPoint, LastIndex, ESplineCoordinateSpace::World); // Adds Spline Endpoint
	
	return SplineComponent;
}
