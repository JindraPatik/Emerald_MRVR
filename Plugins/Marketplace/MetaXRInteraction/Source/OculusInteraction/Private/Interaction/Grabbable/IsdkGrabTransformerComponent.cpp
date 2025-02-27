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

#include "Interaction/Grabbable/IsdkGrabTransformerComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

UIsdkGrabTransformerComponent::UIsdkGrabTransformerComponent()
{
  PrimaryComponentTick.bCanEverTick = false;
}

FIsdkCancelGrabEventDelegate* UIsdkGrabTransformerComponent::GetCancelGrabEventDelegate()
{
  return &CancelGrabEvent;
}

FIsdkGrabTransformerEventDelegate* UIsdkGrabTransformerComponent::GetGrabTransformerEventDelegate()
{
  return &GrabTransformerEvent;
}

void UIsdkGrabTransformerComponent::BeginPlay()
{
  Super::BeginPlay();
  GrabPoses.ClearPoses();

  // Add Transform Target from name if there is no Transform Target
  if (IsValid(TransformTarget))
  {
    SetTransformTarget(TransformTarget);
  }
  else if (TransformTargetName != NAME_None)
  {
    TArray<USceneComponent*> Children;
    GetOwner()->GetRootComponent()->GetChildrenComponents(true, Children);
    Children.Add(GetOwner()->GetRootComponent());
    for (USceneComponent* Child : Children)
    {
      const auto ChildName = Child->GetFName();
      if (ChildName == TransformTargetName)
      {
        SetTransformTarget(Child);
        break;
      }
    }
  }

  if (bIsThrowable && !IsValid(ThrowableComponent))
  {
    ThrowableComponent = Cast<UIsdkThrowable>(GetOwner()->AddComponentByClass(
        UIsdkThrowable::StaticClass(), false, FTransform::Identity, true));
    ThrowableComponent->TrackedComponent = nullptr;
    ThrowableComponent->Settings = ThrowSettings;
    GetOwner()->FinishAddComponent(ThrowableComponent, false, FTransform::Identity);
  }
}

void UIsdkGrabTransformerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  EndTransform();
  Super::EndPlay(EndPlayReason);
}

void UIsdkGrabTransformerComponent::ProcessPointerEvent(const FIsdkInteractionPointerEvent& Event)
{
  if (GrabPoses.GetSelectPoses().Num() >= 1 && Event.Type == EIsdkPointerEventType::Select)
  {
    if (GrabType == EIsdkGrabType::SingleGrabFirstRetained)
    {
      // Cancel new pointer event and stop the new select event
      CancelGrabEvent.Broadcast(Event.Identifier, this);
      return;
    }
    else if (GrabType == EIsdkGrabType::SingleGrabTransferToSecond)
    {
      // Cancel all current pointer events so only the new event is used
      ForceCancel();
    }
  }

  GrabPoses.UpdatePoseArrays(Event);

  switch (Event.Type)
  {
    case EIsdkPointerEventType::Select:
    case EIsdkPointerEventType::Unselect:
    case EIsdkPointerEventType::Cancel:
      EndTransform();
      // Calling "BeginTransform" here because "Select", "Unselect", and "Cancel" change the number
      // of grab poses changes, so the grab transformer could go from a None to Single or Single to
      // Multi, and vice versa. It is possible that after a "Unselect" or "Cancel" the state goes to
      // None in which case the "BeginTransform" function would not trigger any "Transformer" as it
      // checks the number of grab points before activation.
      BeginTransform();
      break;
    case EIsdkPointerEventType::Move:
      UpdateTransform();
      break;
    default:
      break;
  }
}

void UIsdkGrabTransformerComponent::ForceCancel()
{
  const TArray<FIsdkGrabPose> PoseIdentifiers = GrabPoses.GetSelectPoses();
  GrabPoses.ClearPoses();
  // End transform here should be called after the grab poses are removed to make sure the reset
  // physics state code is executed
  EndTransform();
  for (const FIsdkGrabPose& Pose : PoseIdentifiers)
  {
    CancelGrabEvent.Broadcast(Pose.Identifier, this);
  }
}

void UIsdkGrabTransformerComponent::SetSingleGrabTransformer(
    TScriptInterface<IIsdkITransformer> Transformer)
{
  SingleGrabTransformer = Transformer;
  if (IsValid(SingleGrabTransformer.GetObject()))
  {
    SingleGrabTransformer->UpdateConstraints(TargetInitialTransform);
  }
  RestartTransformer();
}

TScriptInterface<IIsdkITransformer> UIsdkGrabTransformerComponent::GetSingleGrabTransformer() const
{
  return SingleGrabTransformer;
}

void UIsdkGrabTransformerComponent::SetMultiGrabTransformer(
    TScriptInterface<IIsdkITransformer> Transformer)
{
  MultiGrabTransformer = Transformer;
  if (IsValid(MultiGrabTransformer.GetObject()))
  {
    MultiGrabTransformer->UpdateConstraints(TargetInitialTransform);
  }
  RestartTransformer();
}

TScriptInterface<IIsdkITransformer> UIsdkGrabTransformerComponent::GetMultiGrabTransformer() const
{
  return MultiGrabTransformer;
}

void UIsdkGrabTransformerComponent::SetTransformTarget(USceneComponent* NewTarget)
{
  TransformTarget = NewTarget;
  TargetInitialTransform = FIsdkTargetTransform(TransformTarget);
  UpdateTransformerConstraints();
}

USceneComponent* UIsdkGrabTransformerComponent::GetTransformTarget() const
{
  return TransformTarget;
}

const FIsdkGrabPoseCollection& UIsdkGrabTransformerComponent::GetGrabPoses() const
{
  return GrabPoses;
}

const TScriptInterface<IIsdkITransformer> UIsdkGrabTransformerComponent::GetActiveGrabTransformer()
    const
{
  return ActiveGrabTransformer;
}

void UIsdkGrabTransformerComponent::BeginTransform()
{
  if (!IsValid(TransformTarget))
  {
    return;
  }

  int SelectCount = GrabPoses.GetSelectPoses().Num();

  switch (SelectCount)
  {
    case 0:
      ActiveGrabTransformer = {};
      break;
    case 1:
      ActiveGrabTransformer = SingleGrabTransformer;
      break;
    default:
      ActiveGrabTransformer = MultiGrabTransformer;
      break;
  }

  if (!IsValid(ActiveGrabTransformer.GetObject()))
  {
    return;
  }

  // If a transformer was activated then stop the physics simulation
  // and cache its current state
  if (!bWasPhysicsCached)
  {
    bWasPhysicsCached = true;
    auto bIsSimulatingPhysics = TransformTarget->IsSimulatingPhysics();
    if (bIsSimulatingPhysics)
    {
      auto* TargetPrimitiveComponent = Cast<UPrimitiveComponent>(TransformTarget);
      if (IsValid(TargetPrimitiveComponent))
      {
        const auto Velocity = TargetPrimitiveComponent->GetPhysicsLinearVelocity();
        TargetPrimitiveComponent->AddForce(-Velocity);
        TargetPrimitiveComponent->SetSimulatePhysics(false);
        TargetPrimitiveComponent->SetEnableGravity(false);
      }
    }
    bWasSimulatingPhysics = bIsSimulatingPhysics;
  }

  if (bIsThrowable && IsValid(ThrowableComponent))
  {
    ThrowableComponent->TrackedComponent = TransformTarget;
  }

  ActiveGrabTransformer->BeginTransform(
      GrabPoses.GetSelectPoses(), FIsdkTargetTransform(TransformTarget));
  GrabTransformerEvent.Broadcast(TransformEvent::BeginTransform, this);
}

void UIsdkGrabTransformerComponent::UpdateTransform()
{
  if (!IsValid(TransformTarget))
  {
    return;
  }

  if (!IsValid(ActiveGrabTransformer.GetObject()))
  {
    return;
  }

  auto TargetRelativeTransform = ActiveGrabTransformer->UpdateTransform(
      GrabPoses.GetSelectPoses(), FIsdkTargetTransform(TransformTarget));
  TransformTarget->SetRelativeTransform(TargetRelativeTransform);
  GrabTransformerEvent.Broadcast(TransformEvent::UpdateTransform, this);
}

void UIsdkGrabTransformerComponent::EndTransform()
{
  if (!IsValid(TransformTarget))
  {
    return;
  }

  if (!IsValid(ActiveGrabTransformer.GetObject()))
  {
    return;
  }

  auto TargetRelativeTransform =
      ActiveGrabTransformer->EndTransform(FIsdkTargetTransform(TransformTarget));
  TransformTarget->SetRelativeTransform(TargetRelativeTransform);
  GrabTransformerEvent.Broadcast(TransformEvent::EndTransform, this);

  ActiveGrabTransformer = {};

  // Target was released
  if (GrabPoses.GetSelectPoses().IsEmpty())
  {
    bWasPhysicsCached = false;
    auto* TargetPrimitiveComponent = Cast<UPrimitiveComponent>(TransformTarget);
    if (IsValid(TargetPrimitiveComponent))
    {
      TargetPrimitiveComponent->SetSimulatePhysics(bWasSimulatingPhysics);

      // Throw
      if (bIsThrowable && IsValid(ThrowableComponent))
      {
        const FVector ReleaseVelocity = ThrowableComponent->GetVelocity();
        const FVector AngularVelocity = ThrowableComponent->GetAngularVelocity().Euler();

        TargetPrimitiveComponent->SetPhysicsLinearVelocity(ReleaseVelocity);
        TargetPrimitiveComponent->SetPhysicsAngularVelocityInDegrees(AngularVelocity);
        TargetPrimitiveComponent->SetEnableGravity(bEnableGravityWhenThrown);

        ThrowableComponent->TrackedComponent = nullptr;
      }
    }
  }
}

void UIsdkGrabTransformerComponent::RestartTransformer()
{
  EndTransform();
  BeginTransform();
}

const void UIsdkGrabTransformerComponent::UpdateTransformerConstraints()
{
  if (IsValid(SingleGrabTransformer.GetObject()))
  {
    SingleGrabTransformer->UpdateConstraints(TargetInitialTransform);
  }
  if (IsValid(MultiGrabTransformer.GetObject()))
  {
    MultiGrabTransformer->UpdateConstraints(TargetInitialTransform);
  }
}
