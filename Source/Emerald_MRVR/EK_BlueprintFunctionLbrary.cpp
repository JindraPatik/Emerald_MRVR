#include "EK_BlueprintFunctionLbrary.h"
#include "PathPoint.h"
#include "SplineComponent.h"
#include "Kismet/GameplayStatics.h"

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

TArray<APathPoint*> UEK_BlueprintFunctionLbrary::SortPathPoints(UObject* WorldContextObject, TSubclassOf<APathPoint> PathPointClass, bool bIsReversed)
{
	if (!WorldContextObject) return {};

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return {};

	TArray<AActor*> FoundActors; // All actors in the World
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, AActor::StaticClass(), FoundActors); 

	TArray<APathPoint*> AllPathPoints; // All PathPoints of selected type in the world
	for (AActor* FoundedActor : FoundActors)
	{
		APathPoint* PathPoint = Cast<APathPoint>(FoundedActor);
		if (PathPoint)
		{
			AllPathPoints.Add(PathPoint);
		}
	}

	TArray<APathPoint*> SortedPathPoints;

	if (!AllPathPoints.IsEmpty())
	{
		int32 PointsCount = AllPathPoints.Num();
		
		if (bIsReversed)
		{
			for (APathPoint* PathPoint : AllPathPoints)
			{
				PathPoint->PathIndex = PointsCount - PathPoint->PathIndex; // Flip all PathIndexes
				SortedPathPoints.Add(PathPoint);
			}
		}
	}
	
	return SortedPathPoints;
}
