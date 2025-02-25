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

#include "IsdkExternalHandDataSource.h"
#include "IsdkHandData.h"
#include "IsdkExternalHandDataModifier.generated.h"

/* Hand Data Source intended to take and modify an existing external HandData Source */
UCLASS(
    ClassGroup = (InteractionSDK),
    Blueprintable,
    meta = (BlueprintSpawnableComponent, DisplayName = "ISDK External Hand Data Modifier"))
class OCULUSINTERACTION_API UIsdkExternalHandDataModifier : public UIsdkExternalHandDataSource
{
  GENERATED_BODY()
 public:
  virtual void TickComponent(
      float DeltaTime,
      ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;
  virtual void OnModify_Implementation(UIsdkHandData* InputHandData);

  /* Get the Hand Data Source this Modifier should be taking in as input */
  UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = InteractionSDK)
  UIsdkHandDataSource* GetInputDataSource() const
  {
    return InputDataSource;
  }

  /* Called when this ticks if all data sources are valid, broadcasts a OnHandDataModified delegate
   * if bound */
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractionSDK)
  void OnModify(UIsdkHandData* InputHandData);

  /* Sets the hand data source this modifier should use as input */
  UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = InteractionSDK)
  void SetInputDataSource(UIsdkHandDataSource* InInputDataSource);

  /* Delegate broadcast during Tick, if all data sources are valid */
  DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandDataModified, UIsdkHandDataSource*, HandData);
  UPROPERTY(BlueprintAssignable)
  FOnHandDataModified OnHandDataModified;

  /* Hand Data Source used as input for this modifier */
  UPROPERTY(
      BlueprintGetter = GetInputDataSource,
      BlueprintSetter = SetInputDataSource,
      EditAnywhere,
      Category = InteractionSDK)
  UIsdkHandDataSource* InputDataSource = nullptr;
};
