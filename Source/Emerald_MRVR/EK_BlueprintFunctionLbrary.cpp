#include "EK_BlueprintFunctionLbrary.h"

#include "EngineUtils.h"
#include "PathPoint.h"
#include "SplineComponent.h"

USplineComponent* UEK_BlueprintFunctionLbrary::CreateSplinePath(UObject* WorldContextObject, FVector StartPoint,
                                                                FVector EndPoint, TArray<APathPoint*> PathPoints, AActor* Outer)
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
		for (APathPoint* PathPoint : PathPoints)
		{
			int32 Index = PathPoint->PathIndex;
			SplineComponent->AddSplinePointAtIndex(PathPoint->GetActorLocation(), Index, ESplineCoordinateSpace::World);
		}
	}

	int32 LastIndex = SplineComponent->GetNumberOfSplinePoints() -1;
	SplineComponent->AddSplinePointAtIndex(EndPoint, LastIndex, ESplineCoordinateSpace::World); // Adds Spline Endpoint

	// SplineComponent->AddSplinePoint(EndPoint, ESplineCoordinateSpace::World);
	
	return SplineComponent;
}

TArray<APathPoint*> UEK_BlueprintFunctionLbrary::SortPathPoints(UObject* WorldContextObject, TSubclassOf<APathPoint> PathPointClass, bool bIsReversed)
{
	if (!WorldContextObject) return {};

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return {};

	TArray<APathPoint*> AllPathPoints; // All PathPoints of selected type in the world
	AllPathPoints.Empty();
	
	for (TActorIterator<APathPoint> It(World); It; ++It)
	{
		APathPoint* PathPoint = *It;
		if (PathPoint && PathPoint->IsA(PathPointClass))
		{
			AllPathPoints.Add(PathPoint);
		}
	}

	TArray<APathPoint*> SortedPathPoints;

	if (!AllPathPoints.IsEmpty())
	{
		int32 PointsCount = AllPathPoints.Num();
		APathPoint* NewPathPoint = nullptr;
		
		if (bIsReversed)
		{
			for (APathPoint* PathPoint : AllPathPoints)
			{
				NewPathPoint->PathIndex = PointsCount - PathPoint->PathIndex; // Flip all PathIndexes
				SortedPathPoints.Add(NewPathPoint);
			}
		}
		else
		{
			SortedPathPoints = AllPathPoints;
		}
	}

	return SortedPathPoints;
}
