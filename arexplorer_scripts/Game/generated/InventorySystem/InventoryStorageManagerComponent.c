 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class InventoryStorageManagerComponent: GameComponent
 {
     proto external IEntity GetOwner();
     proto external bool CanInsertItem(IEntity item, EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY);
     proto external bool CanInsertItemInStorage(IEntity item, BaseInventoryStorageComponent storage, int slotID = -1);
     proto external bool CanInsertResource(ResourceName resourceName, EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY);
     proto external bool CanInsertResourceInStorage(ResourceName resourceName, BaseInventoryStorageComponent storage, int slotID = -1);
     proto external bool TryInsertItem(IEntity item, EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY, InventoryOperationCallback cb = null);
     proto external bool TryInsertItemInStorage(IEntity item, BaseInventoryStorageComponent storage, int slotID = -1, InventoryOperationCallback cb = null);
     proto external bool CanReplaceItem(IEntity item, BaseInventoryStorageComponent storage, int slotID);
     proto external bool TryReplaceItem(IEntity item, BaseInventoryStorageComponent storage, int slotID, InventoryOperationCallback cb = null);
     proto external bool CanRemoveItemFromStorage(IEntity item, BaseInventoryStorageComponent storage);
     proto external bool TryRemoveItemFromStorage(IEntity item, BaseInventoryStorageComponent storage, InventoryOperationCallback cb = null);
     proto external bool CanMoveItemToStorage(IEntity item, BaseInventoryStorageComponent to, int slotID = -1);
     proto external bool TryMoveItemToStorage(IEntity item, BaseInventoryStorageComponent to, int slotID = -1, InventoryOperationCallback cb = null);
     proto external bool CanSwapItemStorages(IEntity itemA, IEntity itemB);
     proto external bool TrySwapItemStorages(IEntity itemA, IEntity itemB, InventoryOperationCallback cb = null);
     proto external bool TryDeleteItem(IEntity item, InventoryOperationCallback cb = null);
     proto external bool TrySpawnPrefabToStorage(ResourceName prefab, BaseInventoryStorageComponent storage = null, int slotID = -1,EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY, InventoryOperationCallback cb = null);
     proto external int GetStorages(out notnull array<BaseInventoryStorageComponent> outStorages, EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY);
     proto external int GetItems(out notnull array<IEntity> outItems);
     proto external bool Contains(IEntity item);
     proto external BaseInventoryStorageComponent FindStorageForItem(IEntity item, EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY);
     proto external BaseInventoryStorageComponent FindStorageForResource(ResourceName resourceName, EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY);
     proto external BaseInventoryStorageComponent FindStorageForInsert(IEntity item, BaseInventoryStorageComponent fromStorage, EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY);
     proto external BaseInventoryStorageComponent FindStorageForResourceInsert(ResourceName resourceName, BaseInventoryStorageComponent fromStorage, EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY);
     proto external int GetDepositItemCountByEntity(IEntity entity);
     proto external int GetDepositItemCountByResource(ResourceName resourceName);
     proto external IEntity FindItem(InventorySearchPredicate predicate, EStoragePurpose purpose = EStoragePurpose.PURPOSE_DEPOSIT);
     proto external int FindItems(out notnull array<IEntity> foundItems, InventorySearchPredicate predicate, EStoragePurpose purpose = EStoragePurpose.PURPOSE_DEPOSIT);
     proto external IEntity FindItemWithComponents(array<typename> componentsQuery, EStoragePurpose purpose = EStoragePurpose.PURPOSE_DEPOSIT);
     proto external int FindItemsWithComponents(out notnull array<IEntity> foundItems, array<typename> componentsQuery, EStoragePurpose purpose = EStoragePurpose.PURPOSE_DEPOSIT);
     proto external int CountItem(InventorySearchPredicate predicate, EStoragePurpose purpose = EStoragePurpose.PURPOSE_DEPOSIT);
     proto external int GetMagazineCountByWeapon(BaseWeaponComponent weapon);
     proto external int GetMagazineCountByMuzzle(BaseMuzzleComponent pMuzzle);
     proto external int GetGrenadesCount();

     // callbacks

     event protected void OnItemAdded(BaseInventoryStorageComponent storageOwner, IEntity item);
     event protected void OnItemRemoved(BaseInventoryStorageComponent storageOwner, IEntity item);
     event protected void OnStorageAdded(BaseInventoryStorageComponent storage);
     event protected void OnStorageRemoved(BaseInventoryStorageComponent storage);
     event protected void FillInitialStorages(out array<BaseInventoryStorageComponent> storagesToAdd);
     event protected void FillInitialPrefabsToStore(out array<ResourceName> prefabsToSpawn);
     event protected bool ShouldForbidRemoveByInstigator(InventoryStorageManagerComponent instigatorManager, BaseInventoryStorageComponent fromStorage, IEntity item) { return false; };
 }
