 [ComponentEditorProps(category: "GameScripted/Editor", description: "Management of placeable entities. Works only with SCR_EditorBaseEntity!", icon: "WBData/ComponentEditorProps/componentEditor.png")]
 class SCR_ContentBrowserEditorComponentClass : SCR_BaseEditorComponentClass
 {
 }


 void ScriptInvoker_BrowserBudgetPreviewRefresh(array<ref SCR_EntityBudgetValue> previewCosts);
 typedef func ScriptInvoker_BrowserBudgetPreviewRefresh;
 typedef ScriptInvokerBase<ScriptInvoker_BrowserBudgetPreviewRefresh> ScriptInvoker_BrowserBudgetPreviewRefreshEvent;

 class SCR_ContentBrowserEditorComponent : SCR_BaseEditorComponent
 {
     [Attribute("1", "If true will show the faction icon on the content browser cards, If false will hide them")]
     protected bool m_bShowFactionOnContentBrowserCards;

     protected const int ASYNC_TICK_LENGTH = 16; //--- How many ticks are allowed per async loading cycle

     /*[Attribute("0", uiwidget: UIWidgets.ComboBox, "Labels that are active by default", "", ParamEnumArray.FromEnum(EEditableEntityLabel))]
     protected ref array<EEditableEntityLabel> m_DefaultActiveLabels; */

     [Attribute(desc: "Content browser menu preset.", defvalue: "-1", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(ChimeraMenuPreset))]
     private ChimeraMenuPreset m_MenuPreset;

     [Attribute("1", desc: "If true it makes sure that the content browser states are loaded from user settings. Else keeps the keeps the sates as defined in m_aContentBrowserTabStates")]
     protected bool m_bUsePersistentBrowserStates;

     [Attribute(desc: "A list of saved states of the content browser such as labels and pagination. Index 0 is the saved hand filters.")]
     protected ref array<ref SCR_EditorContentBrowserSaveStateDataUI> m_aContentBrowserTabStates;

     [Attribute(desc: "Entities with blacklisted labels are not displayed in the content browser unless a SCR_EditorContentBrowserDisplayConfig is used when opening the content browser and the label is in the AlwaysActiveLabels", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
     protected ref array<EEditableEntityLabel> m_eBlackListedLabels;

     //~ Saved content browser state when content browser is opened with config
     protected ref map<ref SCR_EditorContentBrowserDisplayConfig, ref SCR_EditorContentBrowserSaveStateData> m_mContentBrowserConfigStates = new map<ref SCR_EditorContentBrowserDisplayConfig, ref SCR_EditorContentBrowserSaveStateData>;

     protected ref set<ref SCR_EditableEntityCache> m_aExtendedEntitiesCache = new set<ref SCR_EditableEntityCache>();

     protected ref SCR_EditorContentBrowserDisplayConfig m_ContentBrowserConfig;

     private ref array<EEditableEntityLabel> m_ActiveLabels = {};

     protected ref array<ref SCR_EditableEntityCoreLabelGroupSetting> m_LabelGroups = {};

     //~ An array of all the hidden state tabs
     protected ref array<int> m_aHiddenStateTabs = {};

     protected ref array<int> m_aFilteredPrefabIDs = {};
     protected ref array<string> m_aLocalizationKeys = {};
     protected int m_iFilteredPrefabIDsCount;

     protected SCR_MenuEditorComponent m_EditorMenuManager;
     protected SCR_PlacingEditorComponent m_PlacingManager;
     protected SCR_PlacingEditorComponentClass m_PlacingManagerData;
     protected SCR_BudgetEditorComponent m_BudgetManager;
     protected SCR_PrefabsCacheEditorComponent m_PrefabsCache;
     protected ref array<ref SCR_EditableEntityUIInfo> m_aInfos;
     protected ref array<ResourceName> m_aAsyncPrefabs;
     protected int m_iAsyncIndex;

     protected int m_iBrowserStateIndex = 0;
     protected int m_iPageIndex;
     protected int m_iPageEntryCount = 15;
     protected string m_sCurrentSearchText;
     protected string m_sLastSearchText;
     protected bool m_bShowEntityBudgetCost;

     protected SCR_EditableEntityComponent m_ExtendedEntity;
     protected SCR_EditableEntityCore m_EntityCore;

     protected ref ScriptInvoker Event_OnLabelChanged = new ScriptInvoker;
     protected ref ScriptInvoker Event_OnSearchConfirm = new ScriptInvoker; //~ Sends current search string
     protected ref ScriptInvoker Event_OnBrowserStatesSaved = new ScriptInvoker;
     protected ref ScriptInvoker Event_OnBrowserStatePreload = new ScriptInvoker;
     protected ref ScriptInvoker Event_OnBrowserStateLoaded = new ScriptInvoker;
     protected ref ScriptInvoker Event_OnBrowserEntriesFiltered = new ScriptInvoker;
     protected ref ScriptInvoker Event_OnBrowserStateCleared = new ScriptInvoker;
     protected ref ScriptInvoker Event_OnEnableSaveStateTabs = new ScriptInvoker; //~ Sends enable bool
     protected ref ScriptInvoker Event_OnStateTabVisibilityChanged = new ScriptInvoker; //~ Sends index and if visible or not

     ResourceName GetResourceNamePrefabID(int prefabID)
     {
         if (m_PlacingManagerData)
             return m_PlacingManagerData.GetPrefab(prefabID);
         else
             return ResourceName.Empty;
     }

     int GetFilteredPrefabID(int index)
     {
         if (index >= 0 && index < m_aFilteredPrefabIDs.Count())
         {
             return m_aFilteredPrefabIDs.Get(index);
         }
         return -1;
     }

     int GetFilteredPrefabCount()
     {
         return m_iFilteredPrefabIDsCount;
     }

     //------------------------------------------------------------------------------------------------
  bool AreFactionsShownOnContentCards()
     {
         return m_bShowFactionOnContentBrowserCards;
     }

     //------------------------------------------------------------------------------------------------
  int FilterExtendedSlots()
     {
         SCR_EditableEntityComponent extendedEntity = GetExtendedEntity();
         if (!extendedEntity)
             return 0;

         ResourceName extendedEntityResource = extendedEntity.GetPrefab();
         SCR_EditableEntityCache extendedEntityCache = FindExtendedEntityCache(extendedEntityResource);

         if (!extendedEntityCache)
             return 0;

         set<int> extendableEntities = new set<int>();
         extendedEntityCache.GetExtendedEntities(extendableEntities);
         m_aFilteredPrefabIDs.Clear();

         foreach (int prefabID : extendableEntities)
         {
             m_aFilteredPrefabIDs.Insert(prefabID);
         }

         m_iFilteredPrefabIDsCount = m_aFilteredPrefabIDs.Count();
         return m_iFilteredPrefabIDsCount;
     }

     //------------------------------------------------------------------------------------------------
     bool IsExtendedEntity(notnull SCR_EditableEntityComponent entity)
     {
         return FindExtendedEntityCache(entity.GetPrefab()) != null;
     }

     //------------------------------------------------------------------------------------------------
  int GetValidBlackListedLabels(out notnull array<EEditableEntityLabel> validBlackListLabels)
     {
         if (m_ContentBrowserConfig)
         {
             validBlackListLabels.Clear();

             foreach (EEditableEntityLabel label : m_eBlackListedLabels)
             {
                 //~ only add non "always active" labels
                 if (!m_ContentBrowserConfig.IsAlwaysActiveLabel(label))
                     validBlackListLabels.Insert(label);
             }
         }
         else
         {
             validBlackListLabels.Copy(m_eBlackListedLabels);
         }

         return validBlackListLabels.Count();
     }

     //------------------------------------------------------------------------------------------------
     protected SCR_EditableEntityCache FindExtendedEntityCache(ResourceName extendedEntityResource)
     {
         foreach (SCR_EditableEntityCache entityCache : m_aExtendedEntitiesCache)
         {
             if (entityCache.GetPrefab() == extendedEntityResource)
                 return entityCache;
         }

         set<int> extendedEntities = new set<int>();

         SCR_EditableEntityUIInfo info = null;
         for (int prefabId, count = GetInfoCount(); prefabId < count; prefabId++)
         {
             info = GetInfo(prefabId);
             if (!info)
                 continue;

             if (extendedEntityResource == info.GetSlotPrefab())
                 extendedEntities.Insert(prefabId);
         }

         if (extendedEntities.Count() <= 0)
             return null;

         SCR_EditableEntityCache extendedEntityCache = new SCR_EditableEntityCache();
         m_aExtendedEntitiesCache.Insert(extendedEntityCache);
         extendedEntityCache.SetExtendedEntities(extendedEntities);
         extendedEntityCache.SetPrefab(extendedEntityResource);

         return extendedEntityCache;
     }

     void FilterEntries()
     {
         if (GetExtendedEntity())
             return;

         // Filter labels
         m_aFilteredPrefabIDs.Clear();
         m_aLocalizationKeys.Clear();
         array<EEditableEntityLabel> entityLabels = {};

         array<EEditableEntityLabel> validBlackListLabels = {};
         GetValidBlackListedLabels(validBlackListLabels);

         bool isBlackListed;
         SCR_EditableEntityUIInfo info;
         int count = GetInfoCount();
         for (int i = 0; i < count; i++)
         {
             isBlackListed = false;
             entityLabels.Clear();
             info = GetInfo(i);

             if (!info)
             {
                 continue;
             }

             info.GetEntityLabels(entityLabels);
             if (!IsMatchingToggledLabels(entityLabels))
                 continue;

             //~ Ignore entities with blacklist labels
             foreach (EEditableEntityLabel blackListLabel : validBlackListLabels)
             {
                 if (entityLabels.Contains(blackListLabel))
                 {
                     isBlackListed = true;
                     break;
                 }
             }

             //~ Entity has a blacklisted label so don't show it
             if (isBlackListed)
                 continue;

             m_aFilteredPrefabIDs.Insert(i);

             m_aLocalizationKeys.Insert(info.GetName());
         }

         // Search
         string currentSearch = GetCurrentSearch();
         if (!currentSearch.IsEmpty())
         {
             array<int> searchResultPrefabID = {}, searchResultIndices = {};
             WidgetManager.SearchLocalized(currentSearch, m_aLocalizationKeys, searchResultIndices);

             foreach (int searchResultIndex : searchResultIndices)
             {
                 int prefabID = m_aFilteredPrefabIDs.Get(searchResultIndex);
                 searchResultPrefabID.Insert(prefabID);
             }
             m_aFilteredPrefabIDs.Copy(searchResultPrefabID);
         }

         m_iFilteredPrefabIDsCount = m_aFilteredPrefabIDs.Count();

         Event_OnBrowserEntriesFiltered.Invoke();
     }

     bool isMatchingToggledLabelsOfState(int savedStateIndex)
     {
         if (savedStateIndex >= m_aContentBrowserTabStates.Count())
             return false;

         array<EEditableEntityLabel> activeLabels = {};

         m_aContentBrowserTabStates[savedStateIndex].GetLabels(activeLabels);
         return IsMatchingToggledLabels(activeLabels);
     }

     bool IsMatchingToggledLabels(notnull array<EEditableEntityLabel> entityLabels)
     {
         if (!IsAnyLabelActive())
         {
             return true;
         }
         if (entityLabels.IsEmpty())
         {
             return false;
         }

         array<SCR_EditableEntityCoreLabelSetting> groupLabels = {};
         foreach (SCR_EditableEntityCoreLabelGroupSetting labelGroup : m_LabelGroups)
         {
             EEditableEntityLabelGroup labelGroupType = labelGroup.GetLabelGroupType();

             m_EntityCore.GetLabelsOfGroup(labelGroupType, groupLabels);

             int activeLabels = 0;
             int matchesActive = 0;

             bool needsAllActive = labelGroup.GetRequiresAllLabelsMatch();

             foreach (SCR_EditableEntityCoreLabelSetting entityLabelSetting : groupLabels)
             {
                 EEditableEntityLabel entityLabel = entityLabelSetting.GetLabelType();
                 if (!IsLabelActive(entityLabel))
                 {
                     continue;
                 }
                 activeLabels++;
                 if (entityLabels.Find(entityLabel) != -1)
                 {
                     matchesActive++;
                 }
             }

             if (activeLabels > 0)
             {
                 if (needsAllActive && matchesActive != activeLabels)
                 {
                     return false;
                 }
                 else if (!needsAllActive && matchesActive == 0)
                 {
                     return false;
                 }
             }
         }

         return true;
     }

     bool IsLabelActive(EEditableEntityLabel entityLabel)
     {
         return m_ActiveLabels.Find(entityLabel) != -1;
     }

     bool IsAnyLabelActive()
     {
         return !m_ActiveLabels.IsEmpty();
     }

     void SetLabel(EEditableEntityLabel entityLabel, bool active)
     {
         bool labelActive = IsLabelActive(entityLabel);
         if (active && !labelActive)
         {
             m_ActiveLabels.Insert(entityLabel);
         }
         else if (!active && labelActive)
         {
             m_ActiveLabels.RemoveItem(entityLabel);
         }

         Event_OnLabelChanged.Invoke(entityLabel, active);
     }

     void ResetAllLabels(bool setConfigData = true)
     {
         m_ActiveLabels.Clear();

         if (setConfigData)
             SetConfigLabels();
     }

     void GetActiveLabels(out notnull array<EEditableEntityLabel> activeLabels)
     {
         activeLabels.Copy(m_ActiveLabels);
     }

     int GetActiveLabelCount()
     {
         return m_ActiveLabels.Count();
     }

     void GetLabelGroups(out notnull array<ref SCR_EditableEntityCoreLabelGroupSetting> labelGroups)
     {
         labelGroups = m_LabelGroups;
     }

     bool GetLabelGroupType(EEditableEntityLabel entityLabel, out EEditableEntityLabelGroup groupLabel)
     {
         return m_EntityCore.GetLabelGroupType(entityLabel, groupLabel);
     }

     int GetLabelGroupOrder(EEditableEntityLabelGroup groupLabel)
     {
         return m_EntityCore.GetLabelGroupOrder(groupLabel);
     }

     bool GetLabelsOfGroup(EEditableEntityLabelGroup labelGroupType, out notnull array<SCR_EditableEntityCoreLabelSetting> labels)
     {
         return m_EntityCore.GetLabelsOfGroup(labelGroupType, labels);
     }

     bool GetLabelUIInfo(EEditableEntityLabel entityLabel, out SCR_UIInfo uiInfo)
     {
         return m_EntityCore.GetLabelUIInfo(entityLabel, uiInfo);
     }

     bool GetLabelUIInfoIfValid(EEditableEntityLabel entityLabel, EEditorMode currentMode, out SCR_UIInfo uiInfo)
     {
         return m_EntityCore.GetLabelUIInfoIfValid(entityLabel, currentMode, uiInfo);
     }

     void OrderLabels(inout notnull array<EEditableEntityLabel> activeLabels)
     {
         m_EntityCore.OrderLabels(activeLabels);
     }

     int GetLabelOrderInGroup(EEditableEntityLabel entityLabel)
     {
         EEditableEntityLabelGroup groupLabel;
         GetLabelGroupType(entityLabel, groupLabel);

         array<SCR_EditableEntityCoreLabelSetting> labelsInGroup = new array<SCR_EditableEntityCoreLabelSetting>;
         GetLabelsOfGroup(groupLabel, labelsInGroup);
         if (!labelsInGroup)
             return -1;

         int count = labelsInGroup.Count();

         for (int i = 0; i < count; i++)
         {
             if (labelsInGroup[i].GetLabelType() == entityLabel)
                 return i;
         }

         return -1;
     }


     string GetLabelName(EEditableEntityLabel entityLabel)
     {
         SCR_UIInfo uiInfo;
         if (GetLabelUIInfo(entityLabel, uiInfo))
         {
             return uiInfo.GetName();
         }
         return string.Empty;
     }

     void SetPageIndex(int pageIndex)
     {
         m_iPageIndex = pageIndex;
     }

     int GetPageIndex()
     {
         return m_iPageIndex;
     }

     void SetPageEntryCount(int pageEntryCount)
     {
         m_iPageEntryCount = pageEntryCount;
     }

     int GetPageEntryCount()
     {
         return m_iPageEntryCount;
     }

     void SetBrowserStateIndex(int index)
     {
         m_iBrowserStateIndex = index;
     }

     int GetBrowserStateIndex()
     {
         return m_iBrowserStateIndex;
     }



     void GetHiddenStateTabs(out notnull array<int> hiddenStateTabs)
     {
         hiddenStateTabs.Copy(m_aHiddenStateTabs);
     }

     void SetStateTabVisible(int tabIndex, bool setVisible)
     {
         if (!setVisible && !m_aHiddenStateTabs.Contains(tabIndex))
             m_aHiddenStateTabs.Insert(tabIndex);
         else if (setVisible && m_aHiddenStateTabs.Contains(tabIndex))
             m_aHiddenStateTabs.RemoveItem(tabIndex);
         //~ Already hidden or shown
         else
             return;

         Event_OnStateTabVisibilityChanged.Invoke(tabIndex, setVisible);
     }

     int GetContentBrowserTabStates(out notnull array<SCR_EditorContentBrowserSaveStateDataUI> contentBrowserStates)
     {
         foreach (SCR_EditorContentBrowserSaveStateDataUI state : m_aContentBrowserTabStates)
             contentBrowserStates.Insert(state);

         return contentBrowserStates.Count();
     }

     SCR_EditorContentBrowserSaveStateData GetContentBrowserTabState(int index = -1)
     {
         if (index == -1)
         {
             SCR_EditorContentBrowserSaveStateData state;
             if (GetConfigState(m_ContentBrowserConfig, state))
             {
                 return state;
             }
             index = GetBrowserStateIndex();
         }

         if (index < 0 || index >= m_aContentBrowserTabStates.Count())
             return null;

         return m_aContentBrowserTabStates[index];
     }

     string GetLastSearch()
     {
         return m_sLastSearchText;
     }

     string GetCurrentSearch()
     {
         return m_sCurrentSearchText;
     }

     void SetCurrentSearch(string searchText)
     {
         m_sLastSearchText = m_sCurrentSearchText;
         m_sCurrentSearchText = searchText;
     }

     void SetSaveStateTabsEnabled(bool enabled)
     {
         Event_OnEnableSaveStateTabs.Invoke(enabled);
     }

     bool CanSavePersistentBrowserStates()
     {
         return !GetContentBrowserDisplayConfig();
     }

     ScriptInvoker GetOnLabelChanged()
     {
         return Event_OnLabelChanged;
     }

     ScriptInvoker GetOnBrowserStatesSaved()
     {
         return Event_OnBrowserStatesSaved;
     }

     ScriptInvoker GetOnBrowserStatePreload()
     {
         return Event_OnBrowserStatePreload;
     }

     ScriptInvoker GetOnBrowserStateLoaded()
     {
         return Event_OnBrowserStateLoaded;
     }

     ScriptInvoker GetOnBrowserEntriesFiltered()
     {
         return Event_OnBrowserEntriesFiltered;
     }

     ScriptInvoker GetOnBrowserStateCleared()
     {
         return Event_OnBrowserStateCleared;
     }

     ScriptInvoker GetOnEnableSaveStateTabs()
     {
         return Event_OnEnableSaveStateTabs;
     }

     ScriptInvoker GetOnSearchConfirm()
     {
         return Event_OnSearchConfirm;
     }

     ScriptInvoker GetOnStateTabVisibilityChanged()
     {
         return Event_OnStateTabVisibilityChanged;
     }

     void SetExtendedEntity(SCR_EditableEntityComponent extendedEntity)
     {
         m_ExtendedEntity = extendedEntity;
     }

     SCR_EditableEntityComponent GetExtendedEntity()
     {
         return m_ExtendedEntity;
     }

     int GetInfoCount()
     {
         if (!m_aInfos)
             return 0;

         return m_aInfos.Count();
     }

     SCR_EditableEntityUIInfo GetInfo(int index)
     {
         if (m_aInfos && index >= 0 && index < m_aInfos.Count())
         {
             return m_aInfos[index];
         }
         else
         {
             return null;
         }
     }

     int FindIndexOfInfo(SCR_EditableEntityUIInfo info)
     {
         if (m_aInfos && info)
         {
             foreach (int i, SCR_EditableEntityUIInfo browserInfo : m_aInfos)
             {
                 if (browserInfo && browserInfo.GetName() == info.GetName())
                 {
                     return i;
                 }
             }
         }
         return -1;
     }

     bool GetFirstAvailabeBudgetCost(int prefabID, out SCR_EntityBudgetValue budgetCost)
     {
         SCR_EditableEntityUIInfo prefabInfo = GetInfo(prefabID);
         if (!prefabInfo)
             return false;

         array<ref SCR_EntityBudgetValue> budgetCosts = {};
         m_BudgetManager.GetEntityPreviewBudgetCosts(prefabInfo, budgetCosts);

         if (!budgetCosts.IsEmpty())
             budgetCost = budgetCosts.Get(0);
         return budgetCost != null;
     }

     bool CanPlace(int prefabID, out notnull array<ref SCR_EntityBudgetValue> budgetCosts, out SCR_UIInfo blockingBudgetInfo, bool showNotification = false)
     {
         if (!m_BudgetManager)
             return true;

         SCR_EditableEntityUIInfo prefabInfo = GetInfo(prefabID);
         if (!prefabInfo)
             return false;

         EEditableEntityBudget blockingBudget;
         if (m_BudgetManager.CanPlaceEntityInfo(prefabInfo, budgetCosts, blockingBudget, showNotification))
         {
             return true;
         }
         else
         {
             m_BudgetManager.GetCurrentBudgetInfo(blockingBudget, blockingBudgetInfo);
             return false;
         }
     }

     bool CanPlace(notnull array<ResourceName> prefabs, EEditableEntityType fallbackBudgetType = -1)
     {
         //~ No budget manager so get default one (Ignoring mode)
         if (!m_BudgetManager)
             m_BudgetManager = SCR_BudgetEditorComponent.Cast(FindEditorComponent(SCR_BudgetEditorComponent, false, false));

         if (!m_PlacingManager || !m_BudgetManager)
             return false;

         int prefabId;
         Resource resource;
         IEntitySource source;
         array<ref SCR_EntityBudgetValue> fullBudgetCosts = {};
         array<ref SCR_EntityBudgetValue> budgetCosts = {};

         foreach (ResourceName prefab : prefabs)
         {
             prefabId = m_PlacingManager.GetPrefabID(prefab);

             //~ Grab budget from content browser
             if (!m_BudgetManager.GetEntityPreviewBudgetCosts(GetInfo(prefabId), budgetCosts))
             {
                 //~ Grab budget from entity if not registered in content browser
                 if (!m_BudgetManager.GetEntitySourcePreviewBudgetCosts(SCR_EditableEntityComponentClass.GetEditableEntitySource(Resource.Load(prefab)), budgetCosts))
                 {
                     //~ Grab default budget for type if has fall back  and no budget was assigned to entity
                     if (fallbackBudgetType != -1)
                         m_BudgetManager.GetEntityTypeBudgetCost(fallbackBudgetType, budgetCosts);
                 }
             }

             //~ Add found budgets to full list
             foreach (SCR_EntityBudgetValue budget : budgetCosts)
                 fullBudgetCosts.Insert(new SCR_EntityBudgetValue(budget.GetBudgetType(), budget.GetBudgetValue()));

             budgetCosts.Clear();
         }

         //~ Merge Budgets
         SCR_EntityBudgetValue.MergeBudgetCosts(budgetCosts, fullBudgetCosts);

         EEditableEntityBudget blockingBudget;

         //~ Check if can place
         return m_BudgetManager.CanPlace(budgetCosts, blockingBudget);
     }

     bool RefreshPreviewCost(int prefabID = -1)
     {
         if (m_PlacingManager.HasPlacingFlag(EEditorPlacingFlags.CHARACTER_PLAYER) || prefabID == -1)
         {
             m_BudgetManager.ResetPreviewCost();
             return false;
         }

         SCR_EditableEntityUIInfo editableUIInfo = GetInfo(prefabID);
         if (!editableUIInfo)
         {
             return false;
         }

         array<ref SCR_EntityBudgetValue> budgetCosts = {};
         if (m_BudgetManager.GetEntityPreviewBudgetCosts(editableUIInfo, budgetCosts))
         {
             m_BudgetManager.UpdatePreviewCost(budgetCosts);
             return true;
         }

         return false;
     }

     static bool OpenBrowserInstance(int browserStateIndex = -1)
     {
         SCR_ContentBrowserEditorComponent contentBrowserManager = SCR_ContentBrowserEditorComponent.Cast(SCR_ContentBrowserEditorComponent.GetInstance(SCR_ContentBrowserEditorComponent, true));
         if (contentBrowserManager)
             return contentBrowserManager.OpenBrowser(browserStateIndex);
         else
             return false;
     }

     static bool OpenBrowserInstance(SCR_EditorContentBrowserSaveStateData saveState)
     {
         SCR_ContentBrowserEditorComponent contentBrowserManager = SCR_ContentBrowserEditorComponent.Cast(SCR_ContentBrowserEditorComponent.GetInstance(SCR_ContentBrowserEditorComponent, true));
         if (contentBrowserManager)
             return contentBrowserManager.OpenBrowserState(saveState);
         else
             return false;
     }

     static bool OpenBrowserLabelConfigInstance(SCR_EditorContentBrowserDisplayConfig contentBrowserConfig = null)
     {
         SCR_ContentBrowserEditorComponent contentBrowserManager = SCR_ContentBrowserEditorComponent.Cast(SCR_ContentBrowserEditorComponent.GetInstance(SCR_ContentBrowserEditorComponent, true));
         if (contentBrowserManager)
             return contentBrowserManager.OpenBrowserLabelConfig(contentBrowserConfig);
         else
             return false;
     }

     bool OpenBrowserExtended(SCR_EditableEntityComponent extendedEntity, SCR_EditorContentBrowserDisplayConfig contentBrowserConfig = null)
     {
         if (!m_PlacingManager || m_PlacingManager.IsPlacing())
             return false;

         SCR_CompositionSlotManagerComponent slotManager = SCR_CompositionSlotManagerComponent.GetInstance();
         if (slotManager && slotManager.IsOccupied(extendedEntity.GetOwner()))
             return false;

         SCR_EditableEntityComponent currentExtendedEntity = GetExtendedEntity();

         if (contentBrowserConfig == null)
             SetExtendedEntity(extendedEntity);
         else
             SetExtendedEntity(null);

         int extendedPrefabCount = 0;

         if (extendedEntity && !contentBrowserConfig)
             extendedPrefabCount = FilterExtendedSlots();
         else
             extendedPrefabCount = m_iFilteredPrefabIDsCount;

         if (extendedPrefabCount == 0)
         {
             SetExtendedEntity(null);
             return false;
         }

         if ((!contentBrowserConfig && OpenBrowser()) || (contentBrowserConfig && OpenBrowserLabelConfig(contentBrowserConfig)))
         {
             EEditorTransformVertical verticalMode = EEditorTransformVertical.SEA;
             SCR_PreviewEntityEditorComponent previewManager = SCR_PreviewEntityEditorComponent.Cast(SCR_PreviewEntityEditorComponent.GetInstance(SCR_PreviewEntityEditorComponent, false));
             if (previewManager)
                 verticalMode = previewManager.GetVerticalMode();

             vector transform[4];
             extendedEntity.GetTransform(transform);
             SCR_EditorPreviewParams params = SCR_EditorPreviewParams.CreateParams(transform, verticalMode: verticalMode);

             //~ Extened with slot
             if (extendedEntity.GetEntityType() == EEditableEntityType.SLOT || extendedEntity.HasEntityFlag(EEditableEntityFlag.SLOT))
             {
                 params.m_TargetInteraction = EEditableEntityInteraction.SLOT;
             }
             //~ If ever passengers picking can be chosen then this logic needs to change
             else if (extendedEntity.GetOwner().FindComponent(SCR_BaseCompartmentManagerComponent))
             {
                 transform[3] = extendedEntity.GetOwner().GetOrigin() + BaseCompartmentSlot.SPAWN_IN_VEHICLE_OFFSET; //~ Spawn close so the entities are spawned in but still above the entity out of sight
                 params.m_TargetInteraction = EEditableEntityInteraction.PASSENGER;
                 params.m_Parent = extendedEntity;
                 params.m_ParentID = Replication.FindId(extendedEntity);
             }

             params.m_vTransform = transform;
             params.SetTarget(extendedEntity);
             m_PlacingManager.SetInstantPlacing(params);
             return true;
         }

         return false;
     }

     bool OpenBrowser(int browserStateIndex = -1)
     {
         if (!m_EditorMenuManager)
             return false;

         //~ Clear config as browser is opened without it
         m_ContentBrowserConfig = null;
         if (browserStateIndex > 0)
         {
             SetBrowserState(browserStateIndex);
         }

         //~ Open context browser
         OpenBrowserMenu();
         return true;
     }

     bool OpenBrowserState(SCR_EditorContentBrowserSaveStateData saveState, SCR_EditorContentBrowserDisplayConfig displayConfig = null)
     {
         if (!m_EditorMenuManager)
             return false;

         //~ Clear config as browser is opened without it
         if (displayConfig)
             m_ContentBrowserConfig = displayConfig;
         else
             m_ContentBrowserConfig = null;

         LoadBrowserState(saveState, false);
         SaveBrowserState();

         //~ Open context browser
         OpenBrowserMenu();
         return true;
     }

     bool OpenBrowserLabelConfig(SCR_EditorContentBrowserDisplayConfig contentBrowserConfig)
     {
         if (!contentBrowserConfig)
             return OpenBrowser();

         if (!m_EditorMenuManager)
             return false;

         SetExtendedEntity(null);
         m_ContentBrowserConfig = contentBrowserConfig;
         SetBrowserState(contentBrowserConfig);

         //~ Open context browser
         OpenBrowserMenu();
         return true;
     }

     void SetBrowserState(int browserStateIndex = -1, bool saveCurrentState = false, bool clearConfigData = true)
     {
         if (saveCurrentState)
             SaveBrowserState();

         if (browserStateIndex < 0)
             LoadBrowserState(m_iBrowserStateIndex, clearConfigData);
         else
         {
             SetBrowserStateIndex(browserStateIndex);
             LoadBrowserState(browserStateIndex, clearConfigData);
         }
     }

     void SetCustomBrowserState(SCR_EditorContentBrowserSaveStateData state, bool clearConfigData = true)
     {
         LoadBrowserState(state, false);
     }

     void SetBrowserState(SCR_EditorContentBrowserDisplayConfig contentBrowserConfig, bool saveCurrentState = false)
     {
         m_ContentBrowserConfig = contentBrowserConfig;
         SCR_EditorContentBrowserSaveStateData state;
         if (GetConfigState(contentBrowserConfig, state))
             LoadBrowserState(state, false);
         else
             ClearBrowserState(false);
     }

     bool SaveBrowserState()
     {
         SCR_EditorContentBrowserSaveStateData state;

         //~ Does not use config so use default logic
         if (!m_ContentBrowserConfig)
         {
             state = GetContentBrowserTabState(m_iBrowserStateIndex);

             //~ Invalid
             if (!state)
                 return false;
         }
         //~ Uses config and allows for content browser state saving
         else if (!m_ContentBrowserConfig.GetSaveContentBrowserState() || !GetConfigState(m_ContentBrowserConfig, state))
         {
             //~ Do not save state
             return false;
         }

         state.SetLabels(m_ActiveLabels);
         state.SetPageIndex(m_iPageIndex);
         state.SetSearchString(m_sCurrentSearchText);

         Event_OnBrowserStatesSaved.Invoke();

         //~ Saved state successfully
         return true;
     }

     void AddRemoveLabelOfPersistentBrowserState(EEditableEntityLabel label, bool addLabel, int stateIndex = -1)
     {
         if (stateIndex >= m_aContentBrowserTabStates.Count())
             return;

         //~ Only change it on one state
         if (stateIndex > -1)
         {
             m_aContentBrowserTabStates[stateIndex].AddRemoveLabel(label, addLabel);
             return;
         }

         //~ Add to all states
         foreach (SCR_EditorContentBrowserSaveStateData state : m_aContentBrowserTabStates)
             state.AddRemoveLabel(label, addLabel);
     }

     void ResetBrowserState(bool clearConfigData)
     {
         ClearBrowserState(clearConfigData);
         FilterEntries();
     }

     void OnPlaceEntityServer(int prefabID, SCR_EditableEntityComponent entity)
     {
         //m_BudgetManager.CheckMaxBudgetReached(
     }

     void OnMenuClosed()
     {
         if (GetExtendedEntity())
         {
             //ResetBrowserState(false);
             SetExtendedEntity(null);

             // Set back to previous filtering
             LoadBrowserState(m_iBrowserStateIndex, false);
         }

         bool isBuilding = GetManager().GetCurrentMode() == EEditorMode.BUILDING;
         SetBrowserState(-1, true, !isBuilding);

         //~ Save the persistent state
         if (CanSavePersistentBrowserStates())
             SaveAllPersistentBrowserState();
     }

     protected bool OpenBrowserMenu()
     {
         if (m_EditorMenuManager)
         {
             return m_EditorMenuManager.GetMenu().OpenDialog(m_MenuPreset);
         }
         return false;
     }

     //~ Clear browser state
     protected void ClearBrowserState(bool clearConfigData)
     {
         ResetAllLabels(!clearConfigData);
         SetPageIndex(0);
         SetCurrentSearch(string.Empty);

         //~ Clear config
         if (clearConfigData)
             m_ContentBrowserConfig = null;

         Event_OnBrowserStateCleared.Invoke();
     }

     //~ Load the saved browser state using index
     protected void LoadBrowserState(int stateIndex, bool clearConfigData)
     {
         if (stateIndex < 0 || stateIndex >= m_aContentBrowserTabStates.Count())
             return;

         SCR_EditorContentBrowserSaveStateData state = m_aContentBrowserTabStates[stateIndex];
         if (state)
         {
             LoadBrowserState(state, clearConfigData);
         }
     }

     //~ Load the saved browser state
     protected void LoadBrowserState(SCR_EditorContentBrowserSaveStateData state, bool clearConfigData)
     {
         //~ Preload before browser is cleared
         Event_OnBrowserStatePreload.Invoke(state);

         ClearBrowserState(clearConfigData);

         //~ Labels
         array<EEditableEntityLabel> stateActiveLabels = new array<EEditableEntityLabel>;
         state.GetLabels(stateActiveLabels);

         foreach (EEditableEntityLabel activeLabel : stateActiveLabels)
             SetLabel(activeLabel, true);

         //~ Page Index
         SetPageIndex(state.GetPageIndex());

         //~ Search String
         SetCurrentSearch(state.GetSearchString());

         if (!clearConfigData)
             SetConfigLabels();

         //~ Load Event
         Event_OnBrowserStateLoaded.Invoke(state);

         if (m_aInfos && m_aAsyncPrefabs == null)
             FilterEntries();
     }

     protected bool GetConfigState(SCR_EditorContentBrowserDisplayConfig browserConfig, out SCR_EditorContentBrowserSaveStateData state)
     {
         if (!browserConfig)
             return false;

         if (!m_mContentBrowserConfigStates.Find(browserConfig, state))
         {
             state = new SCR_EditorContentBrowserSaveStateData();
             m_mContentBrowserConfigStates.Insert(browserConfig, state);
         }
         return true;
     }

     protected void SetConfigLabels()
     {
         if (!m_ContentBrowserConfig)
             return;

         //~ Set labels active
         array<EEditableEntityLabel> labels = new array<EEditableEntityLabel>;
         m_ContentBrowserConfig.GetAlwaysActiveLabels(labels);

         foreach (EEditableEntityLabel label : labels)
             SetLabel(label, true);
     }

     SCR_EditorContentBrowserDisplayConfig GetContentBrowserDisplayConfig()
     {
         return m_ContentBrowserConfig;
     }

     //~ Save all tab states into settings. So it can be loaded when a diffrent scenario is loaded or the game is booted up again
     void SaveAllPersistentBrowserState()
     {
         if (!m_bUsePersistentBrowserStates)
             return;

         BaseContainer editorSettings = GetGame().GetGameUserSettings().GetModule("SCR_EditorPersistentData");
         if (!editorSettings)
             return;

         array<ref SCR_EditorContentBrowserSaveStateData> settingBrowserStates = {};

         //~ Get the SCR_EditorContentBrowserStateDataUI data
         foreach (SCR_EditorContentBrowserSaveStateData state : m_aContentBrowserTabStates)
         {
             settingBrowserStates.Insert(state);
         }

         //~ Save the Data
         editorSettings.Set("m_aSavedContentBrowserStates", settingBrowserStates);

         GetGame().UserSettingsChanged();
     }

     //~ Load Persistent browser state from where the player left off last time
     protected void LoadAllPersistentBrowserState()
     {
         if (!m_bUsePersistentBrowserStates)
             return;

         BaseContainer editorSettings = GetGame().GetGameUserSettings().GetModule("SCR_EditorPersistentData");
         if (!editorSettings)
             return;

         array<ref SCR_EditorContentBrowserSaveStateData> settingBrowserStates;
         editorSettings.Get("m_aSavedContentBrowserStates", settingBrowserStates);

         if (!settingBrowserStates)
             return;

         int savedCount = settingBrowserStates.Count();

         //~ Check if saved settings are same as given tabs
         if (m_aContentBrowserTabStates.Count() == savedCount)
         {
             array<EEditableEntityLabel> settingSavedLabels = {};

             //~  Get persistant data and apply the data to Browser States
             for (int i = 0; i < savedCount; i++)
             {
                 //~ Note, does not load pageIndex as this might change if mods are added/removed. Plus it might be frustrating/confusing for user
                 settingBrowserStates[i].GetLabels(settingSavedLabels);
                 m_aContentBrowserTabStates[i].SetLabels(settingSavedLabels, true);
                 m_aContentBrowserTabStates[i].SetSearchString(settingBrowserStates[i].GetSearchString());
             }
         }
     }

     override void EOnEditorInit()
     {
         m_EntityCore = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
         m_EntityCore.GetLabelGroups(m_LabelGroups);

         //~ Load Persistent browser state from where the player left off last time
         LoadAllPersistentBrowserState();

         //~ Init the first page, else it will not load correctly
         SetBrowserState(0);
     }

     override void EOnEditorActivateServer()
     {
         m_PrefabsCache = SCR_PrefabsCacheEditorComponent.Cast(FindEditorComponent(SCR_PrefabsCacheEditorComponent, true, false));
         m_PlacingManager = SCR_PlacingEditorComponent.Cast(FindEditorComponent(SCR_PlacingEditorComponent, true, true));

         if (m_PlacingManager)
             m_PlacingManager.GetOnPlaceEntityServer().Insert(OnPlaceEntityServer);
     }

     override void EOnEditorDeactivateServer()
     {
         if (m_PlacingManager)
             m_PlacingManager.GetOnPlaceEntityServer().Remove(OnPlaceEntityServer);
     }

     override void EOnEditorActivate()
     {
         m_EditorMenuManager = SCR_MenuEditorComponent.Cast(FindEditorComponent(SCR_MenuEditorComponent, true, false));
         m_BudgetManager = SCR_BudgetEditorComponent.Cast(FindEditorComponent(SCR_BudgetEditorComponent, true, true));

         m_PlacingManager = SCR_PlacingEditorComponent.Cast(FindEditorComponent(SCR_PlacingEditorComponent, true, true));
         if (m_PlacingManager)
             m_PlacingManagerData = SCR_PlacingEditorComponentClass.Cast(m_PlacingManager.GetEditorComponentData());
     }

     override bool EOnEditorActivateAsync(int attempt)
     {
         //--- Started
         if (attempt == 0)
         {
             SCR_PlacingEditorComponentClass placingPrefabData = SCR_PlacingEditorComponentClass.Cast(SCR_PlacingEditorComponentClass.GetInstance(SCR_PlacingEditorComponent, true));
             if (!placingPrefabData)
                 return true;

             m_aInfos = {};
             m_aAsyncPrefabs = {};
             placingPrefabData.GetPrefabs(m_aAsyncPrefabs, true);
             m_iAsyncIndex = 0;
             return false;
         }

         if (!m_aAsyncPrefabs)
             return true;

         ResourceName entityPrefab;
         Resource entityResource;
         IEntityComponentSource editableEntitySource;
         SCR_EditableEntityUIInfo info;

         int tickEnd = System.GetTickCount() + ASYNC_TICK_LENGTH;
         int count = m_aAsyncPrefabs.Count();
         while (System.GetTickCount() < tickEnd)
         {
             //--- Completed
             if (m_iAsyncIndex >= count)
             {
                 m_aAsyncPrefabs = null;
                 FilterEntries();
                 return true;
             }

             info = null;
             entityPrefab = m_aAsyncPrefabs[m_iAsyncIndex];
             if (entityPrefab)
             {
                 entityResource = BaseContainerTools.LoadContainer(m_aAsyncPrefabs[m_iAsyncIndex]);
                 if (entityResource && entityResource.IsValid())
                 {
                     editableEntitySource = SCR_EditableEntityComponentClass.GetEditableEntitySource(entityResource);
                     if (editableEntitySource)
                     {
                         info = SCR_EditableEntityUIInfo.Cast(SCR_EditableEntityComponentClass.GetInfo(editableEntitySource));
                         if (!info)
                         {
                             Print(string.Format("Prefab '%1' is missing UI info in SCR_EditableEntityComponent!", m_aAsyncPrefabs[m_iAsyncIndex]), LogLevel.ERROR);
                         }
                     }
                     else
                     {
                         Print(string.Format("Prefab '%1' is missing SCR_EditableEntityComponent!", m_aAsyncPrefabs[m_iAsyncIndex]), LogLevel.ERROR);
                     }
                 }
                 else
                 {
                     Print(string.Format("Prefab '%1' is missing at index '%2'!", m_aAsyncPrefabs[m_iAsyncIndex], m_iAsyncIndex), LogLevel.ERROR);
                 }
             }

             //--- Register even when faulty, to keep indexes
             m_aInfos.Insert(info);

             m_iAsyncIndex++;
         }
         return false;
     }

     override void EOnEditorDeactivate()
     {
         m_aInfos = null;
     }

 }
