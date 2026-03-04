
 [BaseContainerProps(configRoot: true), BaseContainerCustomCheckIntTitleField("m_bEnabled", "SpawnerData", "DISABLED - SpawnerData", 1)]
 class SCR_EntityCatalogSpawnerData : SCR_BaseEntityCatalogData
 {
     [Attribute(desc: "UI info overwrite to overwrite entity name. Will use EditableEntityUI if non is set")]
     protected ref SCR_UIInfo m_UiInfo;

     [Attribute("0", desc: "Slot size the entity can be spawned in", uiwidget: UIWidgets.Flags, enums: ParamEnumArray.FromEnum(SCR_EEntitySpawnerSlotType))]
     protected SCR_EEntitySpawnerSlotType m_eSlotTypes;

     [Attribute(defvalue: "1", params: "1 inf", desc: "Prefab entity count. To be used with prefabs like groups, where spawned logic create additional entities")]
     protected int m_iEntityCount;

     [Attribute(desc: "Variants prefab data of entity. These are a diffrent variant of the default")]
     protected ref array<ref SCR_SpawnerVariantData> m_aVariantData;

     protected ResourceName m_sDefaultPrefab;

     protected SCR_ECharacterRank m_eMinimumRequiredRank;

     protected int m_iSupplyCost;

     //--------------------------------- Enable/Disable in Spawner ---------------------------------\\

  void SetEnabled(bool enable)
     {
         m_bEnabled = enable;
     }

     //------------------------------------------------------------------------------------------------
     int GetEntityCount()
     {
         return m_iEntityCount;
     }

     //--------------------------------- Get Overwrite name ---------------------------------\\

  LocalizedString GetOverwriteName()
     {
         if (!m_UiInfo)
             return string.Empty;

         return m_UiInfo.GetName();
     }

     //--------------------------------- Can Spawn in Slot ---------------------------------\\

  bool CanSpawnInSlot(SCR_EEntitySpawnerSlotType slotSize)
     {
         return SCR_Enum.HasFlag(GetValidSlotSizes(), slotSize);
     }

     //--------------------------------- Get Valid slot sizes ---------------------------------\\

  SCR_EEntitySpawnerSlotType GetValidSlotSizes()
     {
         return m_eSlotTypes;
     }

     //--------------------------------- Get has required Rank ---------------------------------\\

  bool HasRequiredRank(SCR_ECharacterRank rank)
     {
         if (rank == SCR_ECharacterRank.INVALID)
             return false;

         return rank >= GetMinimumRequiredRank();
     }

     //--------------------------------- Get minimum required Rank ---------------------------------\\

  SCR_ECharacterRank GetMinimumRequiredRank()
     {
         return m_eMinimumRequiredRank;
     }

     //--------------------------------- Get supply cost ---------------------------------\\

  int GetSupplyCost()
     {
         return m_iSupplyCost;
     }

     //--------------------------------- Get Variants of Type ---------------------------------\\

     ResourceName GetRandomDefaultOrVariantPrefab()
     {
         array<ResourceName> prefabs = {};

         prefabs.Insert(m_sDefaultPrefab);

         foreach (SCR_SpawnerVariantData data: m_aVariantData)
         {
             if (SCR_StringHelper.IsEmptyOrWhiteSpace(data.GetVariantPrefabData()))
                 continue;

             prefabs.Insert(data.GetVariantPrefabData());
         }

         return prefabs.GetRandomElement();
     }

     //--------------------------------- Get Variant data list ---------------------------------\\

  int GetVariantDataList(notnull out array<SCR_SpawnerVariantData> variantData)
     {
         variantData.Clear();

         foreach (SCR_SpawnerVariantData data: m_aVariantData)
         {
             variantData.Insert(data);
         }

         return variantData.Count();
     }

     //--------------------------------- Get Variant Prefab List ---------------------------------\\

  int GetVariantPrefabList(notnull out array<ResourceName> variantPrefabs)
     {
         variantPrefabs.Clear();

         foreach (SCR_SpawnerVariantData data: m_aVariantData)
         {
             variantPrefabs.Insert(data.GetVariantPrefabData());
         }

         return variantPrefabs.Count();
     }

     //--------------------------------- Get Variants of Type ---------------------------------\\

  SCR_SpawnerVariantData GetVariantDataOfType(ESpawnerVariantType variantType)
     {
         foreach (SCR_SpawnerVariantData data: m_aVariantData)
         {
             if (data.GetVariantType() == variantType)
                 return data;
         }

         return null;
     }

     //--------------------------------- Get Variant prefabs of Type ---------------------------------\\

     ResourceName GetVariantPrefabOfType(ESpawnerVariantType variantType)
     {
         SCR_SpawnerVariantData variantData = GetVariantDataOfType(variantType);

         if (!variantData)
             return string.Empty;

         return variantData.GetVariantPrefabData();
     }

     override void InitData(notnull SCR_EntityCatalogEntry entry)
     {
         super.InitData(entry);

         m_sDefaultPrefab = entry.GetPrefab();

         SCR_EditableEntityUIInfo uiInfo = SCR_EditableEntityUIInfo.Cast(entry.GetEntityUiInfo());
         if (!uiInfo)
             return;

         array<ref SCR_EntityBudgetValue> budgets = {};
         uiInfo.GetEntityAndChildrenBudgetCost(budgets);

         m_eMinimumRequiredRank = -1;

         foreach (SCR_EntityBudgetValue budget : budgets)
         {
             switch (budget.GetBudgetType())
             {
                 //~ Set Supply cost
                 case EEditableEntityBudget.CAMPAIGN:
                 {
                     m_iSupplyCost = budget.GetBudgetValue();

                     break;
                 }

                 //~ Set ranks
                 case EEditableEntityBudget.RANK_RENEGADE:
                 {
                     if (m_eMinimumRequiredRank == -1)
                         m_eMinimumRequiredRank = SCR_ECharacterRank.RENEGADE;
                     else
                         Print("'SCR_EntityCatalogSpawnerData' entry " + uiInfo.GetName() + " has rank " + typename.EnumToString(EEditableEntityBudget, m_eMinimumRequiredRank) + " assigned but multiple ranks in the editable UI info which is not supported!");

                     break;
                 }
                 case EEditableEntityBudget.RANK_PRIVATE:
                 {
                     if (m_eMinimumRequiredRank == -1)
                         m_eMinimumRequiredRank = SCR_ECharacterRank.PRIVATE;
                     else
                         Print("'SCR_EntityCatalogSpawnerData' entry " + uiInfo.GetName() + " has rank " + typename.EnumToString(EEditableEntityBudget, m_eMinimumRequiredRank) + " assigned but multiple ranks in the editable UI info which is not supported!");

                     break;
                 }
                 case EEditableEntityBudget.RANK_CORPORAL:
                 {
                     if (m_eMinimumRequiredRank == -1)
                         m_eMinimumRequiredRank = SCR_ECharacterRank.CORPORAL;
                     else
                         Print("'SCR_EntityCatalogSpawnerData' entry " + uiInfo.GetName() + " has rank " + typename.EnumToString(EEditableEntityBudget, m_eMinimumRequiredRank) + " assigned but multiple ranks in the editable UI info which is not supported!");

                     break;
                 }
                 case EEditableEntityBudget.RANK_SERGEANT:
                 {
                     if (m_eMinimumRequiredRank == -1)
                         m_eMinimumRequiredRank = SCR_ECharacterRank.SERGEANT;
                     else
                         Print("'SCR_EntityCatalogSpawnerData' entry " + uiInfo.GetName() + " has rank " + typename.EnumToString(EEditableEntityBudget, m_eMinimumRequiredRank) + " assigned but multiple ranks in the editable UI info which is not supported!");

                     break;
                 }
                 case EEditableEntityBudget.RANK_LIEUTENANT:
                 {
                     if (m_eMinimumRequiredRank == -1)
                         m_eMinimumRequiredRank = SCR_ECharacterRank.LIEUTENANT;
                     else
                         Print("'SCR_EntityCatalogSpawnerData' entry " + uiInfo.GetName() + " has rank " + typename.EnumToString(EEditableEntityBudget, m_eMinimumRequiredRank) + " assigned but multiple ranks in the editable UI info which is not supported!");

                     break;
                 }
                 case EEditableEntityBudget.RANK_CAPTAIN:
                 {
                     if (m_eMinimumRequiredRank == -1)
                         m_eMinimumRequiredRank = SCR_ECharacterRank.CAPTAIN;
                     else
                         Print("'SCR_EntityCatalogSpawnerData' entry " + uiInfo.GetName() + " has rank " + typename.EnumToString(EEditableEntityBudget, m_eMinimumRequiredRank) + " assigned but multiple ranks in the editable UI info which is not supported!");

                     break;
                 }
                 case EEditableEntityBudget.RANK_MAJOR:
                 {
                     if (m_eMinimumRequiredRank == -1)
                         m_eMinimumRequiredRank = SCR_ECharacterRank.MAJOR;
                     else
                         Print("'SCR_EntityCatalogSpawnerData' entry " + uiInfo.GetName() + " has rank " + typename.EnumToString(EEditableEntityBudget, m_eMinimumRequiredRank) + " assigned but multiple ranks in the editable UI info which is not supported!");

                     break;
                 }
                 case EEditableEntityBudget.RANK_COLONEL:
                 {
                     if (m_eMinimumRequiredRank == -1)
                         m_eMinimumRequiredRank = SCR_ECharacterRank.COLONEL;
                     else
                         Print("'SCR_EntityCatalogSpawnerData' entry " + uiInfo.GetName() + " has rank " + typename.EnumToString(EEditableEntityBudget, m_eMinimumRequiredRank) + " assigned but multiple ranks in the editable UI info which is not supported!");

                     break;
                 }
                 case EEditableEntityBudget.RANK_GENERAL:
                 {
                     if (m_eMinimumRequiredRank == -1)
                         m_eMinimumRequiredRank = SCR_ECharacterRank.GENERAL;
                     else
                         Print("'SCR_EntityCatalogSpawnerData' entry " + uiInfo.GetName() + " has rank " + typename.EnumToString(EEditableEntityBudget, m_eMinimumRequiredRank) + " assigned but multiple ranks in the editable UI info which is not supported!");

                     break;
                 }
             }
         }

         //~ No rank set so set minimum rank
         if (m_eMinimumRequiredRank == -1)
             m_eMinimumRequiredRank = SCR_ECharacterRank.PRIVATE;
     }
 };

 //======================================== VARIANT DATA ========================================\\

 [BaseContainerProps(), SCR_BaseContainerCustomTitleResourceName("m_sVariantPrefab", true)]
 class SCR_SpawnerVariantData
 {
     [Attribute(desc: "Prefab of Variant entity", params: "et")]
     protected ResourceName m_sVariantPrefab;

     [Attribute("0", desc: "Type of variant. Type needs to be unique in the list", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(ESpawnerVariantType))]
     protected ESpawnerVariantType m_eVehicleVariantType;

     ResourceName GetVariantPrefabData()
     {
         return m_sVariantPrefab;
     }

     ESpawnerVariantType GetVariantType()
     {
         return m_eVehicleVariantType;
     }

 };

 //======================================== ENUMS  ========================================\\

 enum ESpawnerVariantType
 {
     VEHICLE_MERC,
 };