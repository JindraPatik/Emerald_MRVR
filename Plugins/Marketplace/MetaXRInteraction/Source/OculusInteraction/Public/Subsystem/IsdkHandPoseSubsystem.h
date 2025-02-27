/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * Licensed under the Oculus SDK License Agreement (the "License");
 * you may not use the Oculus SDK except in compliance with the License,
 * which is provided at the time of installation or download, or which
 * otherwise accompanies this software in either electronic or hard copy form.
 *
 * You may obtain a copy of the License at
 *
 * https://developer.oculus.com/licenses/oculussdk/
 *
 * Unless required by applicable law or agreed to in writing, the Oculus SDK
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Subsystems/WorldSubsystem.h"
#include "IsdkContentAssetPaths.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Engine/SkinnedAsset.h"
#include "Engine/SkeletalMesh.h"
#include "IsdkHandData.h"
#include "IsdkHandPoseSubsystem.generated.h"

UCLASS(Abstract)
class OCULUSINTERACTION_API UIsdkHandPoseSubsystemBase : public UWorldSubsystem
{
  GENERATED_BODY()
};

class FIsdkHandPoseSubsystemCollection : public FSubsystemCollection<UIsdkHandPoseSubsystemBase>
{
};

// TODO Make this a configuration instead of always hardcoded
struct FStaticObjectFinders
{
  ConstructorHelpers::FObjectFinderOptional<USkeletalMesh> LeftHandMeshFinder;
  ConstructorHelpers::FObjectFinderOptional<USkeletalMesh> RightHandMeshFinder;
  ConstructorHelpers::FObjectFinderOptional<UMaterial> HandMaterialFinder;

  FStaticObjectFinders()
      : LeftHandMeshFinder(IsdkContentAssetPaths::Models::Hand::OpenXRLeftHand),
        RightHandMeshFinder(IsdkContentAssetPaths::Models::Hand::OpenXRRightHand),
        HandMaterialFinder(IsdkContentAssetPaths::Models::Hand::OculusHandTestMaterial)
  {
  }
};

/**
 * Holds asset references for Hand Pose visualization, active information about hand pose
 * comparisons
 */
UCLASS()
class OCULUSINTERACTION_API UIsdkHandPoseSubsystem : public UIsdkHandPoseSubsystemBase
{
  GENERATED_BODY()

 public:
  UIsdkHandPoseSubsystem();
  virtual void BeginDestroy() override;

  // USubsystem implementation Begin
  virtual void Initialize(FSubsystemCollectionBase& Collection) override;
  // USubsystem implementation End

  /**
   * Static helper function to get an Interaction SDK subsystem from a world.
   * Will fail (via check) if the subsystem does not exist on the given world.
   */
  static UIsdkHandPoseSubsystem& Get(const UWorld* InWorld)
  {
    checkf(IsValid(InWorld), TEXT("World passed to UIsdkHandPoseSubsystem::Get was nullptr"));
    UIsdkHandPoseSubsystem* Instance = InWorld->GetSubsystem<UIsdkHandPoseSubsystem>();
    checkf(
        IsValid(Instance),
        TEXT("Failed to find a UIsdkHandPoseSubsystem for the world %s"),
        *InWorld->GetDebugDisplayName());

    return *Instance;
  }

  USkinnedAsset* GetDebugHandMesh(EIsdkHandType& Handedness);
  UMaterial* GetDebugHandMaterial();

 private:
  TObjectPtr<UMaterial> DebugHandMaterial;
  TObjectPtr<USkinnedAsset> DebugHandMeshRight;
  TObjectPtr<USkinnedAsset> DebugHandMeshLeft;
};
