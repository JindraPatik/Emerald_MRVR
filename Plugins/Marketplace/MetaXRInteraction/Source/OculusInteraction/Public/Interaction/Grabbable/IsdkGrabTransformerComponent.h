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
#include "Components/ActorComponent.h"
#include "../Pointable/IsdkInteractionPointerEvent.h"
#include "IsdkITransformer.h"
#include "IsdkIGrabbable.h"
#include "../IsdkThrowable.h"
#include "IsdkGrabTransformerComponent.generated.h"

UENUM(Blueprintable)
enum class TransformEvent : uint8
{
  BeginTransform,
  UpdateTransform,
  EndTransform
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FIsdkGrabTransformerEventDelegate,
    TransformEvent,
    Event,
    const UIsdkGrabTransformerComponent*,
    GrabTransformer);

/**
 * UIsdkGrabTransformer handles transform and throw behavior for grabbable components.  It does not
 * provide or handle the detection of grab events.  It works only if there is at least one
 * UIsdkGrabbableComponent to detect and pass along grab events.
 *
 * @see UIsdkGrabbableComponent
 * @see IIsdkIGrabbable
 */
UCLASS(
    ClassGroup = (InteractionSDK),
    meta = (BlueprintSpawnableComponent, DisplayName = "Grab Transformer Component"))
class OCULUSINTERACTION_API UIsdkGrabTransformerComponent : public UActorComponent,
                                                            public IIsdkIGrabbable
{
  GENERATED_BODY()

 public:
  // Sets default values for this component's properties
  UIsdkGrabTransformerComponent();

  virtual FIsdkCancelGrabEventDelegate* GetCancelGrabEventDelegate() override;
  virtual void ProcessPointerEvent(const FIsdkInteractionPointerEvent& Event) override;

  /**
   * Sends a cancel event to all interactors that are currently grabbing this grab transformer
   */
  UFUNCTION(BlueprintCallable, Category = InteractionSDK)
  void ForceCancel();

  /**
   * Set the transformer responsible for handling transform logic when a single
   * grabber is interacting with this grab transformer.
   */
  UFUNCTION(BlueprintSetter, Category = InteractionSDK)
  void SetSingleGrabTransformer(TScriptInterface<IIsdkITransformer> Transformer);

  /**
   * Get the transformer responsible for handling transform logic when a single
   * grabber is interacting with this grab transformer.
   */
  UFUNCTION(
      BlueprintPure,
      Category = InteractionSDK,
      meta = (ReturnDisplayName = "SingleGrabTransformer"))
  TScriptInterface<IIsdkITransformer> GetSingleGrabTransformer() const;

  /**
   * Set the transformer responsible for handling transform logic when multiple
   * grabbers are interacting with this grab transformer.
   */
  UFUNCTION(BlueprintCallable, Category = InteractionSDK)
  void SetMultiGrabTransformer(TScriptInterface<IIsdkITransformer> Transformer);

  /**
   * Get the transformer responsible for handling transform logic when multiple
   * grabbers are interacting with this grab transformer.
   */
  UFUNCTION(
      BlueprintPure,
      Category = InteractionSDK,
      meta = (ReturnDisplayName = "MultiGrabTransformer"))
  TScriptInterface<IIsdkITransformer> GetMultiGrabTransformer() const;

  /**
   * Set the target component around which transforms are calculated.
   */
  UFUNCTION(BlueprintCallable, Category = InteractionSDK)
  void SetTransformTarget(USceneComponent* NewTarget);

  /**
   * Get the target component around which transforms are calculated.
   */
  UFUNCTION(
      BlueprintPure,
      Category = InteractionSDK,
      meta = (ReturnDisplayName = "TransformTarget"))
  USceneComponent* GetTransformTarget() const;

  /**
   * Gets a list of grab poses, which represent the points at which this grab transformer
   * is being grabbed.
   */
  UFUNCTION(BlueprintPure, Category = InteractionSDK, meta = (ReturnDisplayName = "GrabPoses"))
  const FIsdkGrabPoseCollection& GetGrabPoses() const;

  /**
   * Get the transformer which is currently driving transform logic
   */
  UFUNCTION(
      BlueprintPure,
      Category = InteractionSDK,
      meta = (ReturnDisplayName = "ActiveGrabTransformer"))
  const TScriptInterface<IIsdkITransformer> GetActiveGrabTransformer() const;

  FIsdkGrabTransformerEventDelegate* GetGrabTransformerEventDelegate();

 protected:
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  void BeginTransform();
  void UpdateTransform();
  void EndTransform();

  FIsdkTargetTransform TargetInitialTransform;

  /**
   * If specified, TransformTargetName is used to search all the components on this actor.
   * If a component with a matching name is found, it will be set as the TransformTarget.
   */
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InteractionSDK")
  FName TransformTargetName;

  /**
   * GrabType specifies how this grab transformer should respond to interaction with
   * multiple grabbers.
   */
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InteractionSDK")
  EIsdkGrabType GrabType = EIsdkGrabType::MultiGrab;

  /**
   * If enabled, the grabbed object will have its velocity and angular velocity set
   * based on its current motion when released.
   */
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InteractionSDK|Throwable")
  bool bIsThrowable = true;

  /**
   * If enabled, the grabbed object will have its gravity automatically enabled when released.
   */
  UPROPERTY(
      BlueprintReadWrite,
      EditAnywhere,
      meta = (EditCondition = "bIsThrowable == true", EditConditionHides),
      Category = "InteractionSDK|Throwable")
  bool bEnableGravityWhenThrown = true;

 private:
  /**
   * GrabTransformerEvent fires when any transform has taken place
   */
  UPROPERTY(BlueprintAssignable, Category = InteractionSDK)
  FIsdkGrabTransformerEventDelegate GrabTransformerEvent;

  /**
   * CancelGrabEvent fires when a grab has been forcibly canceled
   */
  UPROPERTY(BlueprintAssignable, Category = InteractionSDK)
  FIsdkCancelGrabEventDelegate CancelGrabEvent;

  /**
   * The transformer responsible for handling single-grabber behavior
   */
  UPROPERTY(
      BlueprintSetter = SetSingleGrabTransformer,
      BlueprintGetter = GetSingleGrabTransformer,
      meta = (ExposeOnSpawn = true),
      Category = "InteractionSDK")
  TScriptInterface<IIsdkITransformer> SingleGrabTransformer;

  /**
   * The transformer responsible for handling multiple-grabber behavior
   */
  UPROPERTY(
      BlueprintSetter = SetMultiGrabTransformer,
      BlueprintGetter = GetMultiGrabTransformer,
      meta =
          (ExposeOnSpawn = true,
           EditCondition = "GrabType == EIsdkGrabType::MultiGrab",
           EditConditionHides),
      Category = "InteractionSDK")
  TScriptInterface<IIsdkITransformer> MultiGrabTransformer;

  /**
   * The target component around which transforms take place
   */
  UPROPERTY(
      BlueprintSetter = SetTransformTarget,
      BlueprintGetter = GetTransformTarget,
      meta = (ExposeOnSpawn = true),
      Category = "InteractionSDK")
  TObjectPtr<USceneComponent> TransformTarget;

  /**
   * A collection of information about current grabs
   */
  UPROPERTY(BlueprintGetter = GetGrabPoses, Category = "InteractionSDK")
  FIsdkGrabPoseCollection GrabPoses;

  /**
   * The transformer that is actively driving the transformation of this grab transformer
   */
  TScriptInterface<IIsdkITransformer> ActiveGrabTransformer = nullptr;

  /**
   * Settings that drive the throw behavior of this grab transformer
   */
  UPROPERTY(EditAnywhere, Category = "InteractionSDK|Throwable")
  FIsdkThrowableSettings ThrowSettings;

  UPROPERTY()
  TObjectPtr<UIsdkThrowable> ThrowableComponent;

  bool bWasSimulatingPhysics = false;
  bool bWasPhysicsCached = false;

  // Resets transformer state
  void RestartTransformer();

  const void UpdateTransformerConstraints();
};
