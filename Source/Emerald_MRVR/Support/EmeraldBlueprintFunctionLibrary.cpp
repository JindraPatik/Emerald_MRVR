#include "EmeraldBlueprintFunctionLibrary.h"
#include "EngineUtils.h"
#include "Emerald_MRVR/Actors/UnitMovement/PathPoint.h"
#include "SplineComponent.h"

USplineComponent* UEmeraldBlueprintFunctionLibrary::CreateSplinePath(UObject* WorldContextObject, FVector StartPoint,
																FVector EndPoint, TArray<APathPoint*> PathPoints, AActor* Outer)
{
	if (!WorldContextObject) return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;

	USplineComponent* SplineComponent = NewObject<USplineComponent>(Outer, USplineComponent::StaticClass());
	if (!SplineComponent) return nullptr;

	SplineComponent->RegisterComponent();
	SplineComponent->ClearSplinePoints();

	SplineComponent->AddSplinePointAtIndex(StartPoint, 0, ESplineCoordinateSpace::World);

	if (!PathPoints.IsEmpty())
	{
		for (APathPoint* PathPoint : PathPoints)
		{
			SplineComponent->AddSplinePoint(PathPoint->GetActorLocation(), ESplineCoordinateSpace::World);
		}
	}

	SplineComponent->AddSplinePoint(EndPoint, ESplineCoordinateSpace::World);

	return SplineComponent;
}

TArray<APathPoint*> UEmeraldBlueprintFunctionLibrary::SortPathPoints(UObject* WorldContextObject, TSubclassOf<APathPoint> PathPointClass, bool bIsReversed)
{
	if (!WorldContextObject) return {};

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return {};

	TArray<APathPoint*> AllPathPoints;
	
	for (TActorIterator<APathPoint> It(World); It; ++It)
	{
		APathPoint* PathPoint = *It;
		if (PathPoint && PathPoint->IsA(PathPointClass))
		{
			AllPathPoints.Add(PathPoint);
		}
	}

	if (AllPathPoints.IsEmpty()) return {};

	Algo::Sort(AllPathPoints, [](const APathPoint* A, const APathPoint* B)
	{
		return A->PathIndex < B->PathIndex;
	});

	if (bIsReversed)
	{
		Algo::Reverse(AllPathPoints);
	}

	return AllPathPoints;
}
