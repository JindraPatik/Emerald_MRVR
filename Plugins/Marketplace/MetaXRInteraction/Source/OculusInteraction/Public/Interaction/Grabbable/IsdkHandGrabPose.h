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
#include "Components/SceneComponent.h"
#include "IsdkHandPoseData.h"
#include "IsdkHandGrabPose.generated.h"

class UIsdkHandMeshComponent;

/* SceneComponent indicating where and with what pose a hand grab could snap to */
UCLASS(Blueprintable, ClassGroup = (InteractionSDK), meta = (BlueprintSpawnableComponent))
class OCULUSINTERACTION_API UIsdkHandGrabPose : public USceneComponent
{
  GENERATED_BODY()

 public:
  // Sets default values for this component's properties
  UIsdkHandGrabPose();

  /* Whether or not to show the visualized hand pose data in Editor */
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InteractionSDK")
  bool bShowDebugVisualInEditor = true;

  /* Whether or not to destroy this visualized hand pose when BeginPlay happens */
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InteractionSDK")
  bool bRemoveDebugVisualOnPlay = true;

  /* Hand Pose Data asset to use for this Grab Pose */
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InteractionSDK")
  TObjectPtr<UIsdkHandPoseData> HandPoseData;

  /* Set by the HandPoseData, defaults to Right */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionSDK")
  EIsdkHandType Handedness = EIsdkHandType::HandRight;

  UFUNCTION(CallInEditor, BlueprintCallable, Category = InteractionSDK)
  bool InitializeDebugVisual();

  UFUNCTION(CallInEditor, BlueprintCallable, Category = InteractionSDK)
  void DestroyDebugVisual();

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractionSDK)
  void PostComponentCreated();

  virtual void PostComponentCreated_Implementation();

 protected:
  virtual void OnComponentCreated() override;
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

 private:
  UPROPERTY()
  TObjectPtr<UIsdkHandMeshComponent> DebugHandVisual;
};
