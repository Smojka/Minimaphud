 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class InventoryItemComponent: GameComponent
 {
     ref ScriptInvoker<bool> m_OnLockedStateChangedInvoker = new ScriptInvoker<bool>();

     ref ScriptInvoker<InventoryStorageSlot, InventoryStorageSlot> m_OnParentSlotChangedInvoker = new ScriptInvoker<InventoryStorageSlot, InventoryStorageSlot>();

     proto external IEntity GetOwner();
     proto external bool IsLocked();
     proto external bool IsUserLocked();
     proto external bool IsSystemLocked();
     // Get slot where item is located (returns null if none)
     proto external InventoryStorageSlot GetParentSlot();
     proto external ItemAttributeCollection GetAttributes();
     proto external void HideOwner();
     proto external void ShowOwner();
     proto external void SetTraceable(bool traceable);
     proto external void DisablePhysics();
     proto external void EnablePhysics();
     proto external void ActivateOwner(bool active);
     proto external float GetAdditionalWeight();
     proto external void SetAdditionalWeight(float additionalWeight);
     proto external float GetTotalWeight();
     proto external float GetAdditionalVolume();
     proto external void SetAdditionalVolume(float additionalVolume);
     proto external float GetTotalVolume();
     proto external void PlaceOnGround(IEntity callerEntity = null);
     proto external bool IsHiddenInVicnity();
     proto external UIInfo GetUIInfo();
     proto external bool RequestUserLock(IEntity caller, bool locked);
     proto external BaseItemAttributeData FindAttribute(typename typeName);
     proto external IEntity CreatePreviewEntity(BaseWorld world, int camera);

     // callbacks

     event protected bool ShouldHideInVicinity();
     event protected bool OverridePlacementTransform(IEntity caller, out vector computedTransform[4]);
     event private void OnLockedStateChanged(bool nowLocked) { if (m_OnLockedStateChangedInvoker) m_OnLockedStateChangedInvoker.Invoke(nowLocked); };
     event private void OnParentSlotChanged(InventoryStorageSlot oldSlot, InventoryStorageSlot newSlot) { if (m_OnParentSlotChangedInvoker) m_OnParentSlotChangedInvoker.Invoke(oldSlot, newSlot); };
 }
