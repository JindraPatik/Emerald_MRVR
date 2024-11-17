#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

// Print strings
#define DBG_2S(x) if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT(x));}
#define DBG_5S(x) if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT(x));}
#define DBG_9S(x) if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 9.f, FColor::Yellow, TEXT(x));}

#define DBG_ONE_PARAM_STRING(x, y) if (GEngine) {GEngine->AddOnScreenDebugMessage(0, 5.f,FColor::Purple, FString::Printf(TEXT(x), *y));}
#define DBG_ONE_PARAM(x, y) if (GEngine) {GEngine->AddOnScreenDebugMessage(0, 5.f,FColor::Purple, FString::Printf(TEXT(x), y));}

// Debug spheres
#define DRAW_SPHERE5(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 5.f, 12.f, FColor::Blue);
#define DRAW_SPHERE10(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 10.f, 12.f, FColor::Blue);


