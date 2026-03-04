 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseInventoryStorageComponentClass: InventoryItemComponentClass
 {
 }

 class BaseInventoryStorageComponent: InventoryItemComponent
 {
     proto external sealed protected void SetupSlotHooks(InventoryStorageSlot ownedSlot, int slotID);
     proto external sealed protected void ReleaseSlotHooks(InventoryStorageSlot ownedSlot);
     proto external sealed bool Contains(IEntity item);
     proto external sealed InventoryStorageSlot FindItemSlot(IEntity item);
     proto external int GetSlotsCount();
     proto external sealed InventoryStorageSlot GetSlot(int slotID);
     proto external sealed IEntity Get(int slotID);
     // Returns all stored items in this storage
     proto external sealed int GetAll(out notnull array<IEntity> outItems);
     // Returns storage priority
     proto external sealed int GetPriority();
     proto external sealed EStoragePurpose GetPurpose();
     // Search for slot where item can be inserted
     proto external InventoryStorageSlot FindSuitableSlotForItem(IEntity item);
     proto external float GetOccupiedSpace();
     proto external bool IsCompartment();
     proto external void GetOwnedItems(out notnull array<InventoryItemComponent> outItemsComponents);
     proto external bool GetOwnedStorages(out notnull array<BaseInventoryStorageComponent> outStorages, int depth, bool includeHierarchy);
     proto external bool PerformVolumeValidation(IEntity item, bool includeDimensionValidation = true);
     proto external bool PerformDimensionValidation(IEntity item);
     // convinience method, returns volume calculated from dimension limits
     proto external float GetMaxVolumeCapacity();
     // return dimension limits for storage
     proto external vector GetMaxDimensionCapacity();
     proto external float GetWeightFromResource(ResourceName resourceName);
     proto external bool PerformVolumeAndDimensionValidationForResource(ResourceName resourceName, bool includeDimensionValidation = true);

     // callbacks

     event bool OnOverrideCanStoreItem() { return false; };
     event bool OnOverrideCanStoreResource() { return false; };
     event bool OnOverrideCanRemoveItem() { return false; };
     event bool OnOverrideCanReplaceItem() { return false; };
     event protected void OnAddedToSlot(IEntity item, int slotID);
     event protected void OnRemovedFromSlot(IEntity item, int slotID);
     event protected InventoryStorageSlot GetEmptySlotForItem(IEntity item);
     event bool CanStoreItem(IEntity item, int slotID) { return true; };
     event bool CanStoreResource(ResourceName resourceName, int slotID) { return true; };
     event bool CanRemoveItem(IEntity item) { return true; };
     event bool CanReplaceItem(IEntity nextItem, int slotID) { return true; };
     event protected int GetSlotsCountScr();
     event protected InventoryStorageSlot GetSlotScr(int slotID);
     event protected ref BaseInventoryTask RemoveItem(IEntity item);
     event protected ref BaseInventoryTask InsertItem(IEntity item, int slotID);
     event protected bool ShouldPreviewAttachedItems();
     event protected void OnManagerChanged(InventoryStorageManagerComponent manager);
 }
