﻿#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

// Print strings
#define D2G(x) if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT(x));}
#define D5B(x) if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT(x));}
#define D9Y(x) if (GEngine) {GEngine->AddOnScreenDebugMessage(-1, 9.f, FColor::Yellow, TEXT(x));}

#define D_FLOAT_ONE_PARAM(x, y) if (GEngine) {GEngine->AddOnScreenDebugMessage(0, 5.f,FColor::Purple, FString::Printf(TEXT(x), y));}

// Debug spheres
#define DRAW_SPHERE5(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 5.f, 12.f, FColor::Blue);
#define DRAW_SPHERE10(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 10.f, 12.f, FColor::Blue);


