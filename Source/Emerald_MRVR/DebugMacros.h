#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

// Print strings
#define DBG(x, y) if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, x, FColor::Green, TEXT(y));}

#define DBG_ONE_PARAM_STRING(x, y, z) if (GEngine) {GEngine->AddOnScreenDebugMessage(0, x,FColor::Purple, FString::Printf(TEXT(y), *z));}
#define DBG_ONE_PARAM(x, y, z) if (GEngine) {GEngine->AddOnScreenDebugMessage(0, x, FColor::Purple, FString::Printf(TEXT(y), z));}

// UE Logs
#define QQQ(x) UE_LOG(LogTemp, Warning, TEXT(x));
#define SSS(x, y) UE_LOG(LogTemp, Warning, TEXT(x: %s), y);

// Debug spheres
#define DRAW_SPHERE5(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 5.f, 12.f, FColor::Blue);
#define DRAW_SPHERE10(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 10.f, 12.f, FColor::Blue);


