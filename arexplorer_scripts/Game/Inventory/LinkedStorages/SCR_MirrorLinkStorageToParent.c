 [BaseContainerProps(), BaseContainerCustomStringTitleField("Mirror link storage to parent")]
 class SCR_MirrorLinkStorageToParent : SCR_BaseLinkedStorageLogic
 {
     //------------------------------------------------------------------------------------------------
     protected override void DelayedInit(SCR_UniversalInventoryStorageComponent inventoryStorage)
     {
         IEntity parent = inventoryStorage.GetOwner().GetParent();
         if (!parent)
         {
             Print("'SCR_MirrorLinkStorageToParent DelayedInit()' of: '" + inventoryStorage.GetOwner() + "' is trying to set itself as linked storage but it has no parent!", LogLevel.ERROR);
             return;
         }

         SCR_UniversalInventoryStorageComponent parentInventoryStorage = SCR_UniversalInventoryStorageComponent.Cast(parent.FindComponent(SCR_UniversalInventoryStorageComponent));
         if (!parentInventoryStorage)
         {
             Print("'SCR_MirrorLinkStorageToParent DelayedInit()' of: '" + inventoryStorage.GetOwner() + "' is trying to set itself as linked storage but parent has no SCR_UniversalInventoryStorageComponent!", LogLevel.ERROR);
             return;
         }

         //~ Link self to parent
         parentInventoryStorage.AddLinkedStorage(inventoryStorage);

         //~ Link parent to self
         inventoryStorage.AddLinkedStorage(parentInventoryStorage);
     }
 }