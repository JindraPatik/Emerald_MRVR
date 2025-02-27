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

#include "Interaction/Grabbable/IsdkHandGrabPose.h"
#include "IsdkHandPoseData.h"
#include "Engine/SkeletalMesh.h"
#include "Subsystem/IsdkHandPoseSubsystem.h"
#include "IsdkHandMeshComponent.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UIsdkHandGrabPose::UIsdkHandGrabPose()
{
  PrimaryComponentTick.bCanEverTick = false;
}

void UIsdkHandGrabPose::OnComponentCreated()
{
#if WITH_EDITOR
  if (bShowDebugVisualInEditor)
  {
    InitializeDebugVisual();
  }
#endif
  PostComponentCreated();
}

void UIsdkHandGrabPose::PostComponentCreated_Implementation() {}

bool UIsdkHandGrabPose::InitializeDebugVisual()
{
  // Destroy old one if it exists
  DestroyDebugVisual();

  DebugHandVisual = NewObject<UIsdkHandMeshComponent>(this);

  if (!IsValid(HandPoseData))
  {
    UE_LOG(
        LogOculusInteraction,
        Error,
        TEXT("UIsdkHandGrabPose::InitializeDebugVisual() - HandPoseData is invalid!"));
    return false;
  }

  Handedness = HandPoseData->Handedness;
  DebugHandVisual->SetJointsDataSource(HandPoseData);
  DebugHandVisual->SetMappedBoneNamesFromJointSourceHandedness();

  UWorld* WorldObject = GetWorld();
#if WITH_EDITOR
  WorldObject = GEditor->GetEditorWorldContext().World();
#endif
  UIsdkHandPoseSubsystem& HandPoseSubsystem = UIsdkHandPoseSubsystem::Get(WorldObject);

  DebugHandVisual->SetSkinnedAsset(HandPoseSubsystem.GetDebugHandMesh(Handedness));
  if (!IsValid(DebugHandVisual->GetSkinnedAsset()))
  {
    UE_LOG(
        LogOculusInteraction,
        Error,
        TEXT(
            "UIsdkHandGrabPose::InitializeDebugVisual() - DebugHandVisual Skeletal Mesh is invalid"));
    return false;
  }

  DebugHandVisual->SetMaterial(0, HandPoseSubsystem.GetDebugHandMaterial());
  if (!IsValid(DebugHandVisual->GetMaterial(0)))
  {
    UE_LOG(
        LogOculusInteraction,
        Error,
        TEXT("UIsdkHandGrabPose::InitializeDebugVisual() - DebugHandVisual Material is invalid"));
    return false;
  }

  DebugHandVisual->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
  DebugHandVisual->RegisterComponent();

  return true;
}

void UIsdkHandGrabPose::DestroyDebugVisual()
{
  if (IsValid(DebugHandVisual))
  {
    DebugHandVisual->UnregisterComponent();
    DebugHandVisual->DestroyComponent(false);
    DebugHandVisual = nullptr;
  }
}

void UIsdkHandGrabPose::BeginPlay()
{
  Super::BeginPlay();

  // TODO register with HandPoseSubsystem for hand pose calculations before destroying self

  // Destroy if it exists
  if (bRemoveDebugVisualOnPlay)
  {
    DestroyDebugVisual();
  }
}

void UIsdkHandGrabPose::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
}
