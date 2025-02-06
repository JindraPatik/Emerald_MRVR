#include "EK_FunctionLibrary.h"
#include "Components/SplineComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


USplineComponent* UEK_FunctionLibrary::CreateSplinePath(UObject* WorldContextObject, AActor* Outer,
	FVector StartLocation, FVector EndLocation, bool bIsReversed, TSubclassOf<AActor> PathPointClass)
{
	if (!WorldContextObject) return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;

	USplineComponent* SplineComponent = NewObject<USplineComponent>(Outer, USplineComponent::StaticClass());

	/*SplineComponent->RegisterComponent();
	SplineActor->SetRootComponent(SplineComponent);

	// 2️⃣ Přidání počátečního a koncového bodu
	SplineComponent->ClearSplinePoints();
	SplineComponent->AddSplinePoint(StartLocation, ESplineCoordinateSpace::World);
	SplineComponent->AddSplinePoint(EndLocation, ESplineCoordinateSpace::World);

	// 3️⃣ Vložení mezibodů (PathPoints)
	int32 PathPointsCount = 5; // Můžeš upravit počet bodů
	for (int32 i = 1; i < PathPointsCount; i++)
	{
		float Alpha = float(i) / PathPointsCount;
		FVector NewPoint = FMath::Lerp(StartLocation, EndLocation, Alpha);

		// Reverzování pořadí bodů
		if (bIsReversed)
		{
			Alpha = 1.0f - Alpha;
			NewPoint = FMath::Lerp(EndLocation, StartLocation, Alpha);
		}

		SplineComponent->AddSplinePoint(NewPoint, ESplineCoordinateSpace::World);

		// Spawn PathPoint actor (pokud je třída platná)
		if (PathPointsClass)
		{
			AActor* PathPoint = World->SpawnActor<AActor>(PathPointsClass, NewPoint, FRotator::ZeroRotator);
		}
	}

	// 4️⃣ Aktualizace spliny
	SplineComponent->UpdateSpline();

	return SplineComponent;*/
}
