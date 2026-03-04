 class SCR_UniversalInventoryStorageComponentClass : UniversalInventoryStorageComponentClass
 {
 }

 class SCR_UniversalInventoryStorageComponent : UniversalInventoryStorageComponent
 {
     [Attribute( "0", UIWidgets.EditBox, "How much weight it can carry")]
     protected float m_fMaxWeight;

     [Attribute( "0", UIWidgets.EditBox, "The ID of slots the inserted items will be visible in")]
     protected ref array<int> m_aSlotsToShow;

     [Attribute(desc: "Dictates how the entity gets other storage and sets them as linked storage or how it itself sets it as a linked storage of another storage. Do not use base class!")]
     protected ref SCR_BaseLinkedStorageLogic m_LinkedStorageLogic;

     protected ref array<BaseInventoryStorageComponent> m_aLinkedStorages;

     #ifndef DISABLE_INVENTORY
     private SCR_ItemAttributeCollection m_Attributes;
     protected float m_fWeight;
     protected SCR_InventoryStorageManagerComponent                  pInventoryManager;
     protected static const int                                      MIN_VOLUME_TO_SHOW_ITEM_IN_SLOT = 200000;

     //------------------------------------------------------------------------ USER METHODS ------------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
  float GetMaxLoad()
     {
         return m_fMaxWeight;
     }

     //------------------------------------------------------------------------------------------------
     private SCR_ItemAttributeCollection GetAttributeCollection( IEntity item )
     {
         InventoryItemComponent pItemComp = GetItemComponent( item );
         if( !pItemComp )
             return null;

         return SCR_ItemAttributeCollection.Cast( pItemComp.GetAttributes() );
     }

     //------------------------------------------------------------------------------------------------
     protected InventoryItemComponent GetItemComponent( IEntity pItem )
     {
         return InventoryItemComponent.Cast( pItem.FindComponent( InventoryItemComponent ) );
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsWeightOk( float fWeight )
     {
         if (!m_Attributes)
             return false;

         fWeight += GetTotalWeight() - m_Attributes.GetWeight();

         return m_fMaxWeight >= fWeight;
     }

     //------------------------------------------------------------------------------------------------
     override bool CanStoreItem(IEntity item, int slotID)
     {
         if (!super.CanStoreItem(item, slotID))
             return false;

         InventoryItemComponent pItemComp = GetItemComponent( item );
         if( !pItemComp )
             return false;

         bool bVolumeOK = PerformVolumeValidation( item );
         if( !bVolumeOK )
         {
             if( pInventoryManager )
                 pInventoryManager.SetReturnCode( EInventoryRetCode.RETCODE_ITEM_TOO_BIG );
         }

         bool bWeightOK = IsWeightOk( pItemComp.GetTotalWeight() );
         if( !bWeightOK )
         {
             if( pInventoryManager )
                 pInventoryManager.SetReturnCode( EInventoryRetCode.RETCODE_ITEM_TOO_HEAVY );
         }

         bool bDimensionsOK = PerformDimensionValidation(item);
         return bVolumeOK && bWeightOK && bDimensionsOK;
     }

     //------------------------------------------------------------------------------------------------
     override bool CanStoreResource(ResourceName resourceName, int slotID)
     {
         if (!super.CanStoreResource(resourceName, slotID))
             return false;

         bool bVolumeOK = PerformVolumeAndDimensionValidationForResource(resourceName, true);
         if( !bVolumeOK )
         {
             if( pInventoryManager )
                 pInventoryManager.SetReturnCode( EInventoryRetCode.RETCODE_ITEM_TOO_BIG );
         }

         float fWeight = GetWeightFromResource(resourceName);
         bool bWeightOK = IsWeightOk( fWeight );
         if( !bWeightOK )
         {
             if( pInventoryManager )
                 pInventoryManager.SetReturnCode( EInventoryRetCode.RETCODE_ITEM_TOO_HEAVY );
         }

         return bVolumeOK && bWeightOK;
     }

     //------------------------------------------------------------------------------------------------
     override bool CanReplaceItem(IEntity nextItem, int slotID)
     {
         if (!super.CanReplaceItem(nextItem, slotID))
             return false;

         if (!nextItem)
             return false;

         IEntity item = Get(slotID);

         if (!item)
             return false;

         // item is the item that is getting replaced by nextItem
         // nextItem is the item that is replacing the item at slotID
         // slotID is is the slot ID for the item that is getting replaced by nextItem

         InventoryItemComponent itemComp = GetItemComponent(item);
         if(!itemComp)
             return false;

         InventoryItemComponent nextItemComp = GetItemComponent(nextItem);
         if(!nextItemComp)
             return false;

         float itemVolume = itemComp.GetTotalVolume();
         float nextItemVolume = nextItemComp.GetTotalVolume();
         float occupiedVolumeWithoutItem = GetOccupiedSpace() - itemVolume;

         bool bVolumeOK = occupiedVolumeWithoutItem + nextItemVolume <= GetMaxVolumeCapacity();
         if(!bVolumeOK && pInventoryManager)
         {
             pInventoryManager.SetReturnCode(EInventoryRetCode.RETCODE_ITEM_TOO_BIG);
         }

         bool bWeightOK = IsWeightOk(nextItemComp.GetTotalWeight() - itemComp.GetTotalWeight());
         if(!bWeightOK && pInventoryManager)
         {
             pInventoryManager.SetReturnCode(EInventoryRetCode.RETCODE_ITEM_TOO_HEAVY);
         }

         bool bDimensionsOK = PerformDimensionValidation(nextItem);
         return bVolumeOK && bWeightOK && bDimensionsOK;
     }

     //------------------------------------------------------------------------------------------------
     override void OnRemovedFromSlot(IEntity item, int slotID)
     {
         super.OnRemovedFromSlot(item, slotID);

         GenericEntity pGenComp = GenericEntity.Cast( item );
         InventoryItemComponent pItemComponent = InventoryItemComponent.Cast(pGenComp.FindComponent(InventoryItemComponent));
         pItemComponent.ShowOwner();
         pItemComponent.EnablePhysics();

         m_fWeight -= pItemComponent.GetTotalWeight();
     }

     //------------------------------------------------------------------------------------------------
     protected override void OnAddedToSlot(IEntity item, int slotID)
     {
         super.OnAddedToSlot(item, slotID);

         GenericEntity pGenComp = GenericEntity.Cast( item );
         InventoryItemComponent pItemComponent = InventoryItemComponent.Cast(pGenComp.FindComponent(InventoryItemComponent));
         if( !pItemComponent )
             return;

         float fVol = pItemComponent.GetTotalVolume();
         if ( m_aSlotsToShow.Find( slotID ) != -1 )
         {
                 pItemComponent.ShowOwner();
         }
         else
         {
             if ( fVol >= MIN_VOLUME_TO_SHOW_ITEM_IN_SLOT )
                 pItemComponent.ShowOwner();
         }

         pItemComponent.DisablePhysics();
         pItemComponent.ActivateOwner(false);

         m_fWeight += pItemComponent.GetTotalWeight();
     }

     //------------------------------------------------------------------------ LINKED STORAGES ----------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
  int GetLinkedStorages(notnull out array<BaseInventoryStorageComponent> linkedStorages)
     {
         if (!m_aLinkedStorages)
             return 0;

         linkedStorages.Copy(m_aLinkedStorages);
         return linkedStorages.Count();
     }

     //------------------------------------------------------------------------------------------------
  void AddLinkedStorage(BaseInventoryStorageComponent newLinkedStorage)
     {
         //~ Do not add self as linked storage
         if (newLinkedStorage == this)
             return;

         if (!m_aLinkedStorages)
             m_aLinkedStorages = {};
         //~ Already contains the storage
         else if (m_aLinkedStorages.Contains(newLinkedStorage))
             return;

         m_aLinkedStorages.Insert(newLinkedStorage);
     }

     //------------------------------------------------------------------------------------------------
  bool IsStorageALinkedChild(notnull BaseInventoryStorageComponent storage)
     {
         return m_aLinkedStorages.Contains(storage);
     }

     //------------------------------------------------------------------------ COMMON METHODS ----------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
     override void OnManagerChanged(InventoryStorageManagerComponent manager)
     {
         super.OnManagerChanged(manager);

         pInventoryManager = SCR_InventoryStorageManagerComponent.Cast( manager );
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_UniversalInventoryStorageComponent( IEntityComponentSource src, IEntity ent, IEntity parent )
     {
         m_Attributes = SCR_ItemAttributeCollection.Cast(GetAttributes());
         if (!m_Attributes)
             return;

         m_fWeight = m_Attributes.GetWeight();

         if (m_LinkedStorageLogic)
             m_LinkedStorageLogic.Init(this);
     }
     #else
     private SCR_ItemAttributeCollection GetAttributeCollection( IEntity item );
     protected InventoryItemComponent GetItemComponent( IEntity pItem );
     protected bool IsVolumeOk( float fVolume );
     protected bool IsWeightOk( float fWeight );
     override bool CanStoreItem(IEntity item, int slotID);
     override bool CanStoreResource(ResourceName resourceName, int slotID);
     override bool CanRemoveItem(IEntity item);
     override void OnRemovedFromSlot(IEntity item, int slotID);
     protected override void OnAddedToSlot(IEntity item, int slotID);
     override void OnManagerChanged(InventoryStorageManagerComponent manager);
 //  void SCR_UniversalInventoryStorageComponent( IEntityComponentSource src, IEntity ent, IEntity parent );

     #endif
 }