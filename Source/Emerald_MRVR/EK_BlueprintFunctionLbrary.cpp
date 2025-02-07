#include "EK_BlueprintFunctionLbrary.h"
#include "EngineUtils.h"
#include "PathPoint.h"
#include "SplineComponent.h"

USplineComponent* UEK_BlueprintFunctionLbrary::CreateSplinePath(UObject* WorldContextObject, FVector StartPoint,
                                                                FVector EndPoint, TArray<APathPoint*> PathPoints, bool bIsReversed, AActor* Outer)
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

	SplineComponent->AddSplinePoint(EndPoint, ESplineCoordinateSpace::World);
	
	return SplineComponent;
}

TArray<APathPoint*> UEK_BlueprintFunctionLbrary::SortPathPoints(UObject* WorldContextObject, TSubclassOf<APathPoint> PathPointClass, bool bIsReversed)
{
	if (!WorldContextObject) return {};

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return {};

	
	TArray<APathPoint*> AllPathPoints; // All PathPoints of selected type in the world
	TArray<APathPoint*> SortedPathPoints;
	AllPathPoints.Empty();
	SortedPathPoints.Empty();
	
	for (TActorIterator<APathPoint> It(World); It; ++It)
	{
		APathPoint* PathPoint = *It;
		if (PathPoint && PathPoint->IsA(PathPointClass))
		{
			AllPathPoints.Add(PathPoint);
		}
	}

	int32 PointsCount = AllPathPoints.Num();
	APathPoint* NewPathPoint = nullptr;

	SortedPathPoints = AllPathPoints;


	if (SortedPathPoints.IsEmpty()) return {};

	Algo::Sort(SortedPathPoints, [](const APathPoint* A, const APathPoint* B)
	{
		return A->PathIndex < B->PathIndex;
	});

	if (bIsReversed)
	{
		Algo::Reverse(SortedPathPoints);
	}

	return SortedPathPoints;
}
