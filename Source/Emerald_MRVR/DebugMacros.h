#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

#define D(x) if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT(x));}