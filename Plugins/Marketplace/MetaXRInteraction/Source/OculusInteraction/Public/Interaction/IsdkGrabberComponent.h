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
#include "IsdkSceneInteractorComponent.h"
#include "IsdkGrabberComponent.generated.h"

class UIsdkGrabbableComponent;
class USphereComponent;

USTRUCT()
struct FIsdkColliderInfo
{
  GENERATED_BODY()

  FIsdkColliderInfo();

  EIsdkInteractorState State;

  UPROPERTY(Instanced)
  TMap<UPrimitiveComponent*, UIsdkGrabbableComponent*> HoverObjects;

  UPROPERTY(Instanced)
  TObjectPtr<UIsdkGrabbableComponent> SelectObject;

  // RankIndex is a way to figure which collider has most recently started hovering over a grabbable
  int RankIndex = 0;
};

/**
 * IsdkGrabberComponent drives the ability for a pawn to interact with actors that have an
 * IsdkGrabbableComponent attached to them.  This component uses a number of configurable colliders
 * per grab type (eg, pinch / palm grab) to drive detection of grabbables.  Selection/Unselection
 * is expected to be driven externally by an IsdkGrabInteractionRigComponent.
 */
UCLASS(
    Blueprintable,
    ClassGroup = (InteractionSDK),
    meta = (BlueprintSpawnableComponent, DisplayName = "ISDK Grabber Component"))
class OCULUSINTERACTION_API UIsdkGrabberComponent : public UIsdkSceneInteractorComponent
{
  GENERATED_BODY()

 public:
  UIsdkGrabberComponent();

  virtual void BeginPlay() override;
  virtual void TickComponent(
      float DeltaTime,
      enum ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
  virtual void BeginDestroy() override;
  virtual void Deactivate() override;

  /**
   * Grab (select) any interactables its pinch colliders are hovering over.
   */
  void PinchGrab();

  /**
   * Grab (select) any interactables its palm colliders are hovering over.
   */
  void PalmGrab();

  /**
   * Release (unselect) any interactables selected by pinch colliders.
   */
  void PinchRelease();

  /**
   * Release (unselect) any interactables selected by palm colliders.
   */
  void PalmRelease();

  /**
   * @return true if this grabber has a valid grabbed component
   */
  UFUNCTION(BlueprintPure, Category = InteractionSDK, meta = (ReturnDisplayName = "bIsGrabbing"))
  bool IsGrabbing() const;

  /**
   * @return the component selected (grabbed) by this grabbable
   */
  UFUNCTION(
      BlueprintPure,
      Category = InteractionSDK,
      meta = (ReturnDisplayName = "GrabbedComponent"))
  UIsdkGrabbableComponent* GetGrabbedComponent() const;

  /**
   * @return the component that is being used to select the currently selected grabbable.
   */
  UFUNCTION(
      BlueprintPure,
      Category = InteractionSDK,
      meta = (ReturnDisplayName = "GrabbingComponent"))
  UPrimitiveComponent* GetGrabbingComponent() const;

  /**
   * Get all the components hovered which are hovered by colliders of the given
   * EIsdkGrabColliderType.
   *
   * @param GrabType - The grab type we should get hovered components for
   * @param OutColliders - The hovered components
   */
  UFUNCTION(BlueprintCallable, Category = InteractionSDK)
  void GetCollidersByType(
      EIsdkGrabColliderType GrabType,
      TArray<UPrimitiveComponent*>& OutColliders);

  /**
   * Returns true if any colliders of the given EIsdkGrabColliderType are
   * hovered by this interactor
   *
   * @param GrabType - The grab type we should check hovered components for
   * @return true if the given grab type is hovering over any components
   */
  UFUNCTION(BlueprintCallable, Category = InteractionSDK)
  bool HasCollidersOfType(EIsdkGrabColliderType GrabType);

  /* Adds a PrimitiveComponent of the given EIsdkGrabColliderType to this Interactor */
  UFUNCTION(BlueprintCallable, Category = InteractionSDK)
  void AddCollider(UPrimitiveComponent* Collider, EIsdkGrabColliderType Type);

  /* Removes a PrimitiveComponent of the given EIsdkGrabColliderType from this Interactor */
  UFUNCTION(BlueprintCallable, Category = InteractionSDK)
  void RemoveCollider(UPrimitiveComponent* Collider, EIsdkGrabColliderType Type);

  /**
   * Check to see if this grabber is hovering over anything for any grab type
   * @return true if we are hovering over any interactables
   */
  virtual bool HasInteractable() const override;

  /**
   * Check to see if this grabber is selecting anything for any grab type.
   * @return true if we are selecting any interactables
   */
  virtual bool HasSelectedInteractable() const override;

  /**
   * Set the amount by which the default palm grab collider should be
   * offset from its attach parent.  Used particularly to account for
   * discrepancies between hand and controller offsets from the motion
   * controller root.
   *
   * @param InPalmOffset - The amount by which to offset the default palm grab
   *   collider from its attach parent.
   */
  void UpdatePalmOffset(const FVector& InPalmOffset);

  /**
   * Get the number of components this grabber is hovering over and selecting.
   * @return the number of components this grabber is hovering over and selecting.
   */
  virtual FIsdkInteractionRelationshipCounts GetInteractorStateRelationshipCounts() const override;

  /**
   * Get the components this grabber is interacting with.
   * @param State - Whether to return hovered or selected interactables
   * @param OutInteractables The output interactables this grabber is interacting with.
   */
  virtual void GetInteractorStateRelationships(
      EIsdkInteractableState State,
      TArray<TScriptInterface<IIsdkIInteractableState>>& OutInteractables) const override;

  /**
   * @return true if palm grab is allowed
   */
  bool IsPalmGrabAllowed() const;

  /**
   * @return true if pinch grab is allowed
   */
  bool IsPinchGrabAllowed() const;

 protected:
  /**
   * Attempt to select a grabbable for the given grab type
   * @param GrabType
   */
  void SelectByGrabType(EIsdkGrabColliderType GrabType);

  /**
   * Attempt to unselect any grabbables for the given grab type
   * @param GrabType
   */
  void UnselectByGrabType(EIsdkGrabColliderType GrabType);

  /**
   * The collider which is currently selecting the selected interactable
   */
  UPROPERTY(BlueprintReadOnly, Category = InteractionSDK)
  TObjectPtr<UPrimitiveComponent> SelectingCollider;

  /**
   * Whether or not this Grabber will activate when a palm grab is recognized
   */
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = InteractionSDK)
  bool bAllowPalmGrab = true;

  /**
   * Whether or not this Grabber will activate when a pinch grab is recognized
   */
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = InteractionSDK)
  bool bAllowPinchGrab = true;

  /* Radius of the to set when initializing for the pinch grab overlap collider */
  UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = InteractionSDK)
  float PinchColliderRadius = 1.2f;

  /* Radius of the to set when initializing for the palm grab overlap collider */
  UPROPERTY(
      BlueprintReadOnly,
      EditAnywhere,
      meta = (EditCondition = "bAllowPalmGrab == true", EditConditionHides),
      Category = InteractionSDK)
  float PalmColliderRadius = 4.0f;

 private:
  UFUNCTION(BlueprintInternalUseOnly, Category = InteractionSDK)
  void BeginOverlap(
      UPrimitiveComponent* OverlappedComponent,
      AActor* OtherActor,
      UPrimitiveComponent* OtherComp,
      int32 OtherBodyIndex,
      bool bFromSweep,
      const FHitResult& SweepResult);

  UFUNCTION(BlueprintInternalUseOnly, Category = InteractionSDK)
  void EndOverlap(
      UPrimitiveComponent* OverlappedComponent,
      AActor* OtherActor,
      UPrimitiveComponent* OtherComp,
      int32 OtherBodyIndex);

  void UpdateOverlappedSet();
  virtual void UpdateState();

  bool GetColliderInfo(
      UPrimitiveComponent* Collider,
      EIsdkGrabColliderType GrabType,
      FIsdkColliderInfo& OutColliderInfo);

  void SetColliderInfo(
      UPrimitiveComponent* Collider,
      EIsdkGrabColliderType Type,
      const FIsdkColliderInfo& ColliderInfo);

  void ComputeBestGrabbableColliderForGrabType(
      EIsdkGrabColliderType Type,
      UPrimitiveComponent*& MyCollider,
      UPrimitiveComponent*& GrabbableCollider,
      UIsdkGrabbableComponent*& Grabbable);

  void PostEvent(EIsdkPointerEventType Type, UIsdkGrabbableComponent* Dest);
  void UnregisterAllColliders();
  void UnregisterCollider(UPrimitiveComponent* Collider, EIsdkGrabColliderType Type);

  EIsdkGrabColliderType FindColliderType(UPrimitiveComponent* Collider) const;

  int64 PointerEventToken = 0;

  UPROPERTY(Instanced)
  TObjectPtr<UIsdkGrabbableComponent> GrabbedComponent;

  UPROPERTY()
  TObjectPtr<USphereComponent> DefaultPalmCollider;

  /*
   * In order to store state associated with a collider, we hold a map of collider to
   * collider info.  We then store a separate map for each grab type.
   */
  using FPrimitiveComponentToColliderInfoMap = TMap<UPrimitiveComponent*, FIsdkColliderInfo>;
  using FGrabTypeToColliderInfoMap =
      TMap<EIsdkGrabColliderType, FPrimitiveComponentToColliderInfoMap>;
  FGrabTypeToColliderInfoMap GrabTypeToColliderInfoMap;

  UPROPERTY()
  TSet<UIsdkGrabbableComponent*> OverlappedGrabbables;

  int NextRankIndex = 0;
  EIsdkGrabColliderType SelectingColliderType;

  void DrawDebugVisuals() const;
  FVector GetSelectingColliderPosition() const;

  UFUNCTION()
  void HandleGrabbableCancelEvent(
      int Identifier,
      TScriptInterface<IIsdkIGrabbable> GrabbableSender);
};
