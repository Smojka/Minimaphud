 [BaseContainerProps(), BaseContainerCustomStringTitleField("Set storage as linked to parent")]
 class SCR_SetLinkedStorageToParent : SCR_BaseLinkedStorageLogic
 {
     //------------------------------------------------------------------------------------------------
     protected override void DelayedInit(SCR_UniversalInventoryStorageComponent inventoryStorage)
     {
         IEntity parent = inventoryStorage.GetOwner().GetParent();
         if (!parent)
         {
             Print("'SCR_SetLinkedStorageToParent DelayedInit()' of: '" + inventoryStorage.GetOwner() + "' is trying to set itself as linked storage but it has no parent!", LogLevel.ERROR);
             return;
         }

         SCR_UniversalInventoryStorageComponent parentInventoryStorage = SCR_UniversalInventoryStorageComponent.Cast(parent.FindComponent(SCR_UniversalInventoryStorageComponent));
         if (!parentInventoryStorage)
         {
             Print("'SCR_SetLinkedStorageToParent DelayedInit()' of: '" + inventoryStorage.GetOwner() + "' is trying to set itself as linked storage but parent has no SCR_UniversalInventoryStorageComponent!", LogLevel.ERROR);
             return;
         }

         parentInventoryStorage.AddLinkedStorage(inventoryStorage);
     }
 }