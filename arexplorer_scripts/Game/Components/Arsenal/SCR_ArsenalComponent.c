 class SCR_ArsenalComponentClass : ScriptComponentClass
 {
 }

 void ScriptInvokerArsenalUpdatedMethod(array<ResourceName> arsenalItems);
 typedef func ScriptInvokerArsenalUpdatedMethod;
 typedef ScriptInvokerBase<ScriptInvokerArsenalUpdatedMethod> ScriptInvokerArsenalUpdated;

 class SCR_ArsenalComponent : ScriptComponent
 {
     [Attribute(SCR_EArsenalSupplyCostType.DEFAULT.ToString(), desc: "Cost type of items. If it is not DEFAULT than it will try to get the diffrent supply cost if the item has it assigned" , uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(SCR_EArsenalSupplyCostType), category: "Settings")]
     protected SCR_EArsenalSupplyCostType m_eSupplyCostType;

     [Attribute("", desc: "Toggle supported SCR_EArsenalItemType by this arsenal, items are gathered from SCR_Faction or from the overwrite config", uiwidget: UIWidgets.Flags, enums: ParamEnumArray.FromEnum(SCR_EArsenalItemType), category: "Settings")]
     protected SCR_EArsenalItemType m_eSupportedArsenalItemTypes;

     [Attribute("", desc: "Toggle supported SCR_EArsenalItemMode by this arsenal, items are gathered from SCR_Faction or from the overwrite config", uiwidget: UIWidgets.Flags, enums: ParamEnumArray.FromEnum(SCR_EArsenalItemMode), category: "Settings")]
     protected SCR_EArsenalItemMode m_eSupportedArsenalItemModes;

     [Attribute("", desc: "Toggle which SCR_EArsenalItemType groups are exposed for gamemaster attribute editing", uiwidget: UIWidgets.Flags, enums: ParamEnumArray.FromEnum(SCR_EArsenalAttributeGroup), category: "Settings")]
     protected SCR_EArsenalAttributeGroup m_eEditableAttributeGroups;

     [Attribute(category: "Overwrite", desc: "If empty this will be ignored. For ease of use do not edit this directly in the prefab, use a config instead. The items added the config will allow the arsenal to spawn these items within it. Note that it will still filter on type, meaning that if weapons are added to the list but disabled in the Supported types then they will not show in the arsenal. Changing faction will not have any effect if this is not null")]
     protected ref SCR_ArsenalItemListConfig m_OverwriteArsenalConfig;

     [Attribute("1", desc: "Get default faction if current faction could not be found", category: "Settings")]
     protected bool m_bGetDefaultIfNoFaction;

     [Attribute("0", desc: "If true will try and find the SCR_FactionAffiliationComponent from parent if it was not found on self", category: "Settings")]
     protected bool m_bAllowGetFactionFromParent;

     [Attribute("0", desc: "Save type of Arsenal. Only applicable if there is a save arsenal action attached to the arsenal.\nSAVING_DISABLED: Saving action is disabled.\nIN_ARSENAL_ITEMS_ONLY: Only allow saving if all the items the player has are in the arsenal as well.\nFACTION_ITEMS_ONLY: Only allow saving if all the items the player has in their inventory are of the faction of the arsenal.\nNO_RESTRICTIONS: No restriction as what the arsenal is allowed to save.", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(SCR_EArsenalSaveType), category: "Settings")]
     protected SCR_EArsenalSaveType m_eArsenalSaveType;

     [Attribute("0", desc: "If false it will get the arsenals current faction and change when the faction is updated. If true it will never check the on faction and use the default faction assigned. Use this if you want the content to never change or if there is never a current faction", category: "Settings")]
     protected bool m_bAlwaysUseDefaultFaction;

     protected bool m_bArsenalEnabled = true;
     protected SCR_EArsenalItemMode m_eOnDisableArsenalModes; //~ Saves the arsenal item mode when the arsenal is disabled and the mode is set to 0

     protected bool m_bArsenalSavingDisplayedIfDisabled = true;
     protected bool m_bHasSaveArsenalAction = false;

     protected SCR_ArsenalInventoryStorageManagerComponent m_InventoryComponent;
     protected UniversalInventoryStorageComponent m_StorageComponent;

     protected FactionManager m_FactionManager;
     protected SCR_FactionAffiliationComponent m_FactionComponent;

     protected ref ScriptInvokerArsenalUpdated m_OnArsenalUpdated;

     protected bool m_bIsClearingInventory;

     //------------------------------------------------------------------------------------------------
  bool IsArsenalEnabled()
     {
         return m_bArsenalEnabled;
     }

     //------------------------------------------------------------------------------------------------
  void SetArsenalEnabled(bool enable)
     {
         if (m_bArsenalEnabled == enable)
             return;

         SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
         if ((gameMode && !gameMode.IsMaster()) || (!gameMode && Replication.IsClient()))
             return;

         if (!enable)
         {
             m_eOnDisableArsenalModes = m_eSupportedArsenalItemModes;
             SetSupportedArsenalItemModes(0);
         }
         else
         {
             SetSupportedArsenalItemModes(m_eOnDisableArsenalModes);
         }

         SetArsenalEnabledBroadcast(enable);
         Rpc(SetArsenalEnabledBroadcast, enable);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
     protected void SetArsenalEnabledBroadcast(bool enable)
     {
         m_bArsenalEnabled = enable;
     }

     //------------------------------------------------------------------------------------------------
     SCR_EArsenalSupplyCostType GetSupplyCostType()
     {
         return m_eSupplyCostType;
     }

     //------------------------------------------------------------------------------------------------
  bool IsArsenalUsingSupplies()
     {
         IEntity owner = GetOwner();
         if (!owner)
             return null;

         SCR_ResourceComponent resourceComponent = SCR_ResourceComponent.FindResourceComponent(owner);

         return resourceComponent && resourceComponent.IsResourceTypeEnabled();
     }

     //------------------------------------------------------------------------------------------------
     SCR_EArsenalSaveType GetArsenalSaveType()
     {
         return m_eArsenalSaveType;
     }

     //------------------------------------------------------------------------------------------------
  bool IsArsenalSavingDisplayedIfDisabled()
     {
         return m_bArsenalSavingDisplayedIfDisabled;
     }

     //------------------------------------------------------------------------------------------------
  void SetHasSaveArsenalAction(bool hasSaveArsenalAction)
     {
         m_bHasSaveArsenalAction = hasSaveArsenalAction;
     }

     //------------------------------------------------------------------------------------------------
  bool HasSaveArsenalAction()
     {
         return m_bHasSaveArsenalAction;
     }

     //------------------------------------------------------------------------------------------------
  void SetArsenalSaveType(SCR_EArsenalSaveType saveType)
     {
         if (m_eArsenalSaveType == saveType)
             return;

         RPL_SetArsenalSaveType(saveType);
         Rpc(RPL_SetArsenalSaveType, saveType);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
     void RPL_SetArsenalSaveType(SCR_EArsenalSaveType saveType)
     {
         //~ Nothing changed so make sure the action displayed if disabled is not set
         if (m_eArsenalSaveType == saveType)
             return;

         m_eArsenalSaveType = saveType;

         if (saveType != SCR_EArsenalSaveType.SAVING_DISABLED)
             m_bArsenalSavingDisplayedIfDisabled = true;
     }

     //------------------------------------------------------------------------------------------------
  SCR_EArsenalItemType GetSupportedArsenalItemTypes()
     {
         return m_eSupportedArsenalItemTypes;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_ArsenalComponent FindArsenalComponent(notnull IEntity entity, bool getFromSlotted = true)
     {
         SCR_ArsenalComponent arsenalComponent;

         if (getFromSlotted)
             arsenalComponent = SCR_ArsenalComponent.Cast(SCR_EntityHelper.FindComponent(entity, SCR_ArsenalComponent));
         else
             arsenalComponent = SCR_ArsenalComponent.Cast(entity.FindComponent(SCR_ArsenalComponent));

         return arsenalComponent;
     }

     //------------------------------------------------------------------------------------------------
  void SetSupportedArsenalItemTypes(SCR_EArsenalItemType types)
     {
         m_eSupportedArsenalItemTypes = types;
         RefreshArsenal();
     }

     //------------------------------------------------------------------------------------------------
  SCR_EArsenalItemMode GetSupportedArsenalItemModes()
     {
         return m_eSupportedArsenalItemModes;
     }

     //------------------------------------------------------------------------------------------------
  void SetSupportedArsenalItemModes(SCR_EArsenalItemMode modes)
     {
         m_eSupportedArsenalItemModes = modes;
         RefreshArsenal();
     }

     //------------------------------------------------------------------------------------------------
  SCR_EArsenalAttributeGroup GetEditableAttributeGroups()
     {
         return m_eEditableAttributeGroups;
     }

     //------------------------------------------------------------------------------------------------
     SCR_ArsenalInventoryStorageManagerComponent GetArsenalInventoryComponent()
     {
         return m_InventoryComponent;
     }

     //------------------------------------------------------------------------------------------------
  SCR_ArsenalItemListConfig GetOverwriteArsenalConfig()
     {
         return m_OverwriteArsenalConfig;
     }

     //------------------------------------------------------------------------------------------------
  bool GetAvailablePrefabs(out notnull array<ResourceName> availablePrefabs)
     {
         array<SCR_ArsenalItem> arsenalItems = {};
         if (!GetFilteredArsenalItems(arsenalItems))
         {
             return false;
         }

         for (int i = 0; i < arsenalItems.Count(); i++)
         {
             SCR_ArsenalItem itemToSpawn = arsenalItems[i % arsenalItems.Count()];
             if (!itemToSpawn)
                 continue;

             availablePrefabs.Insert(itemToSpawn.GetItemResourceName());
         }
         return !availablePrefabs.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  SCR_Faction GetAssignedFaction()
     {
         //~ Safty if get assigned faction is called before the init
         if (!m_FactionComponent)
             FactionInit(GetOwner());

         if (m_FactionComponent)
         {
             //~ Always use default faction
             if (m_bAlwaysUseDefaultFaction)
                 return SCR_Faction.Cast(m_FactionComponent.GetDefaultAffiliatedFaction());

             SCR_Faction faction = SCR_Faction.Cast(m_FactionComponent.GetAffiliatedFaction());
             if (!faction && m_bGetDefaultIfNoFaction)
             {
                 faction = SCR_Faction.Cast(m_FactionComponent.GetDefaultAffiliatedFaction());
             }

             return faction;
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  bool GetFilteredArsenalItems(out notnull array<SCR_ArsenalItem> filteredArsenalItems, EArsenalItemDisplayType requiresDisplayType = -1)
     {
         //~ Is overwritting Arsenal
         if (m_OverwriteArsenalConfig)
         {
             GetFilteredOverwriteArsenalItems(filteredArsenalItems, requiresDisplayType);
             return !filteredArsenalItems.IsEmpty();
         }

         //~ Cannot get items if no entity catalog manager
         SCR_EntityCatalogManagerComponent catalogManager = SCR_EntityCatalogManagerComponent.GetInstance();
         if (!catalogManager)
             return false;

         //~ Get filtered list from faction (if any) or default
         filteredArsenalItems = catalogManager.GetFilteredArsenalItems(m_eSupportedArsenalItemTypes, m_eSupportedArsenalItemModes, GetAssignedFaction(), requiresDisplayType);
         return !filteredArsenalItems.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  bool GetFilteredOverwriteArsenalItems(out notnull array<SCR_ArsenalItem> filteredArsenalItems, EArsenalItemDisplayType requiresDisplayType = -1)
     {
         filteredArsenalItems = m_OverwriteArsenalConfig.GetFilteredArsenalItems(m_eSupportedArsenalItemTypes, m_eSupportedArsenalItemModes, requiresDisplayType);
         return !filteredArsenalItems.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  //~ Used by SCR_ArsenalDisplayComponent
     void ClearArsenal()
     {
     }

     //------------------------------------------------------------------------------------------------
  void RefreshArsenal(SCR_Faction faction = null)
     {
         ClearArsenal();

         array<SCR_ArsenalItem> arsenalItems = {};
         if (!GetFilteredArsenalItems(arsenalItems))
             return;

         //SCR_Sorting<SCR_ArsenalItem, SCR_CompareArsenalItemPriority>.HeapSort(arsenalItems);

         array<ResourceName> arsenalPrefabs = {};

         int arsenalItemCount = arsenalItems.Count();
         for (int i = 0; i < arsenalItemCount; i++)
         {
             SCR_ArsenalItem itemToSpawn = arsenalItems[i % arsenalItemCount];
             if (!itemToSpawn)
                 continue;

             arsenalPrefabs.Insert(itemToSpawn.GetItemResourceName());
         }

         if (m_OnArsenalUpdated)
             m_OnArsenalUpdated.Invoke(arsenalPrefabs);

         if (Replication.IsServer())
             Rpc(RPC_OnArsenalUpdated, m_eSupportedArsenalItemTypes, m_eSupportedArsenalItemModes);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
     protected void RPC_OnArsenalUpdated(SCR_EArsenalItemType itemTypes, SCR_EArsenalItemMode itemModes)
     {
         //~ Call later so faction is correctly updated
         GetGame().GetCallqueue().CallLater(DelayedOnArsenalUpdatedClient, param1: itemTypes, param2: itemModes);
     }

     //------------------------------------------------------------------------------------------------
     protected void DelayedOnArsenalUpdatedClient(SCR_EArsenalItemType itemTypes, SCR_EArsenalItemMode itemModes)
     {
         m_eSupportedArsenalItemTypes = itemTypes;
         m_eSupportedArsenalItemModes = itemModes;

         if (!m_FactionComponent)
             FactionInit(GetOwner());

         if (m_OnArsenalUpdated)
         {
             array<ResourceName> arsenalPrefabs = {};
             GetAvailablePrefabs(arsenalPrefabs);

             m_OnArsenalUpdated.Invoke(arsenalPrefabs);
         }
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetItemValid(SCR_EArsenalItemType arsenalItemType, SCR_EArsenalItemMode arsenalItemMode)
     {
         return arsenalItemType & m_eSupportedArsenalItemTypes
             && arsenalItemMode & m_eSupportedArsenalItemModes;
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetItemValid(SCR_Faction faction, int index, out bool isEmpty = true)
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  ScriptInvokerArsenalUpdated GetOnArsenalUpdated()
     {
         if (!m_OnArsenalUpdated)
             m_OnArsenalUpdated = new ScriptInvokerArsenalUpdated();

         return m_OnArsenalUpdated;
     }

     //------------------------------------------------------------------------------------------------
     protected void OnFactionChanged(FactionAffiliationComponent owner, Faction previousFaction, Faction newFaction)
     {
         RefreshArsenal();
     }

     //------------------------------------------------------------------------------------------------
     override protected void OnPostInit(IEntity owner)
     {
         if (SCR_Global.IsEditMode())
             return;

         //~ Make sure it knows that it is disabled at the start
         if (GetArsenalSaveType() == SCR_EArsenalSaveType.SAVING_DISABLED)
             m_bArsenalSavingDisplayedIfDisabled = false;

         SetEventMask(owner, EntityEvent.INIT);

         // Initialize inventory of arsenal, if applicable (Display racks without additional inventory will return here)
         m_InventoryComponent = SCR_ArsenalInventoryStorageManagerComponent.Cast(owner.FindComponent(SCR_ArsenalInventoryStorageManagerComponent));
         m_StorageComponent = UniversalInventoryStorageComponent.Cast(owner.FindComponent(UniversalInventoryStorageComponent));
         // Arsenal inventory is filled after OnPostInit by SCR_ArsenalInventoryStorageManagerComponent.FillInitialPrefabsToStore
     }

     //------------------------------------------------------------------------------------------------
     override protected void EOnInit(IEntity owner)
     {
         m_FactionManager = GetGame().GetFactionManager();
         SCR_FactionAffiliationComponent factionComponent = SCR_FactionAffiliationComponent.Cast(owner.FindComponent(SCR_FactionAffiliationComponent));
         if (!factionComponent && m_bAllowGetFactionFromParent)
             GetGame().GetCallqueue().CallLater(FactionInit, param1: owner);
         else
             FactionInit(owner);

     }

     //------------------------------------------------------------------------------------------------
     protected void FactionInit(IEntity owner)
     {
         //~ Faction is already init
         if (m_FactionComponent)
             return;

         m_FactionComponent = SCR_FactionAffiliationComponent.Cast(owner.FindComponent(SCR_FactionAffiliationComponent));
         if (!m_FactionComponent && m_bAllowGetFactionFromParent)
         {
             if (owner.GetParent())
                 m_FactionComponent = SCR_FactionAffiliationComponent.Cast(owner.GetParent().FindComponent(SCR_FactionAffiliationComponent));

             if (!m_FactionComponent)
                 return;
         }
         else if (!m_FactionComponent)
             return;

         if (!m_bAlwaysUseDefaultFaction)
         {
             RplComponent rplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));
             if (rplComponent && rplComponent.Role() == RplRole.Authority)
             {
                 m_FactionComponent.GetOnFactionChanged().Insert(OnFactionChanged);
             }
         }
     }

     //------------------------------------------------------------------------------------------------
     override protected void OnDelete(IEntity owner)
     {
         if (SCR_Global.IsEditMode())
             return;

         RplComponent rplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));
         if (rplComponent && rplComponent.Role() == RplRole.Authority)
         {
             if (!m_bAlwaysUseDefaultFaction && m_FactionComponent)
                 m_FactionComponent.GetOnFactionChanged().Remove(OnFactionChanged);
         }
     }

     //------------------------------------------------------------------------------------------------
     override bool RplSave(ScriptBitWriter writer)
     {
         writer.WriteInt(m_eSupportedArsenalItemTypes);
         writer.WriteInt(m_eSupportedArsenalItemModes);
         writer.WriteInt(GetArsenalSaveType());
         writer.WriteBool(m_bArsenalEnabled);
         return true;
     }

     //------------------------------------------------------------------------------------------------
     override bool RplLoad(ScriptBitReader reader)
     {
         int itemTypes, itemModes, saveType;
         bool isArsenalEnabled;

         reader.ReadInt(itemTypes);
         reader.ReadInt(itemModes);
         reader.ReadInt(saveType);
         reader.ReadBool(isArsenalEnabled);

         RPC_OnArsenalUpdated(itemTypes, itemModes);
         RPL_SetArsenalSaveType(saveType);
         SetArsenalEnabledBroadcast(isArsenalEnabled);

         return true;
     }
 }

 //------------------------------------------------------------------------------------------------
 enum SCR_EArsenalSaveType
 {
     SAVING_DISABLED = 0,
     IN_ARSENAL_ITEMS_ONLY = 10,
     FACTION_ITEMS_ONLY = 20,
     NO_RESTRICTIONS = 30,
 }

 //------------------------------------------------------------------------------------------------
 enum SCR_EArsenalSupplyCostType
 {
     DEFAULT,
     GADGET_ARSENAL,
     RESPAWN_COST,
 }