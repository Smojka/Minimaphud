 [ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Manager for holding non-faction enities and to getter functions for entities from factions")]
 class SCR_EntityCatalogManagerComponentClass : SCR_BaseGameModeComponentClass
 {
 }

 class SCR_EntityCatalogManagerComponent : SCR_BaseGameModeComponent
 {
     [Attribute(desc: "List of non-faction related Entity catalogs. Each holds a list of entity Prefab and data of a given type. Catalogs of the same type are merged into one. Note this array is moved to a map on init and set to null")]
     protected ref array<ref SCR_EntityCatalog> m_aEntityCatalogs;

     //~ Catalog map for quicker obtaining the catalog using EEntityCatalogType
     protected ref map<EEntityCatalogType, ref SCR_EntityCatalog> m_mEntityCatalogs = new map<EEntityCatalogType, ref SCR_EntityCatalog>();

     //~ Faction manager ref
     protected SCR_FactionManager m_FactionManager;

     //~ Instance
     protected static SCR_EntityCatalogManagerComponent s_Instance;

     protected bool m_bInitDone;

     //------------------------------------------------------------------------------------------------
  static SCR_EntityCatalogManagerComponent GetInstance()
     {
         return s_Instance;
     }

     //======================================== GET CATALOG ========================================\\

     //------------------------------------------------------------------------------------------------
  SCR_EntityCatalog GetEntityCatalogOfType(EEntityCatalogType catalogType)
     {
         if (!m_bInitDone)
         {
             Debug.Error2("SCR_EntityCatalogManagerComponent", "Trying to obtain catalog of type: '" + typename.EnumToString(EEntityCatalogType, catalogType) + "' (Factionless) but catalog is not yet initialized! Call your function one frame later!");
             return null;
         }

         //~ Get catalog
         SCR_EntityCatalog entityCatalog = m_mEntityCatalogs.Get(catalogType);

         if (entityCatalog)
             return entityCatalog;

         //~ No data found
         Print(string.Format("'SCR_EntityCatalogManagerComponent' trying to get entity list of type '%1' but there is no catalog with that type.", typename.EnumToString(EEntityCatalogType, catalogType)), LogLevel.WARNING);
         return null;
     }

     //------------------------------------------------------------------------------------------------
  SCR_EntityCatalog GetFactionEntityCatalogOfType(EEntityCatalogType catalogType, FactionKey factionKey)
     {
         if (!m_FactionManager)
             return null;

         SCR_Faction faction = SCR_Faction.Cast(m_FactionManager.GetFactionByKey(factionKey));
         if (!faction)
         {
             Print(string.Format("'SCR_EntityCatalogManagerComponent', GetFactionEntityCatalogOfType could not get Catalog as faction '%1' is invalid or does not exist in current game", factionKey), LogLevel.ERROR);
             return null;
         }

         return GetFactionEntityCatalogOfType(catalogType, faction);
     }

     //------------------------------------------------------------------------------------------------
  SCR_EntityCatalog GetFactionEntityCatalogOfType(EEntityCatalogType catalogType, notnull SCR_Faction faction)
     {
         return faction.GetFactionEntityCatalogOfType(catalogType);
     }

     //======================================== GET ALL CATALOGS ========================================\\

     //------------------------------------------------------------------------------------------------
  int GetAllEntityCatalogs(notnull out array<SCR_EntityCatalog> outEntityCatalogs)
     {
         outEntityCatalogs.Clear();
         foreach (SCR_EntityCatalog entityCatalog : m_mEntityCatalogs)
         {
             outEntityCatalogs.Insert(entityCatalog);
         }

         return outEntityCatalogs.Count();
     }

     //------------------------------------------------------------------------------------------------
  int GetAllFactionEntityCatalogs(notnull out array<SCR_EntityCatalog> outEntityCatalogs, FactionKey factionKey)
     {
         if (!m_FactionManager)
             return 0;

         SCR_Faction faction = SCR_Faction.Cast(m_FactionManager.GetFactionByKey(factionKey));
         if (!faction)
         {
             Print(string.Format("'SCR_EntityCatalogManagerComponent', GetAllFactionEntityCatalogs could not get Catalog as faction '%1' is invalid or does not exist in current game", factionKey), LogLevel.ERROR);
             return 0;
         }

         return GetAllFactionEntityCatalogs(outEntityCatalogs, faction);
     }

     //------------------------------------------------------------------------------------------------
  int GetAllFactionEntityCatalogs(notnull out array<SCR_EntityCatalog> outEntityCatalogs, notnull SCR_Faction faction)
     {
         return faction.GetAllFactionEntityCatalogs(outEntityCatalogs);
     }

     //======================================== GET SPECIFIC ENTRY WITH PREFAB ========================================\\

     //------------------------------------------------------------------------------------------------
  SCR_EntityCatalogEntry GetEntryWithPrefabFromCatalog(EEntityCatalogType catalogType, ResourceName prefabToFind)
     {
         //~ No prefab given
         if (SCR_StringHelper.IsEmptyOrWhiteSpace(prefabToFind))
             return null;

         //~ Get catalog
         SCR_EntityCatalog catalog = GetEntityCatalogOfType(catalogType);
         if (!catalog)
             return null;

         //~ Try and find prefab
         return catalog.GetEntryWithPrefab(prefabToFind);
     }

     //------------------------------------------------------------------------------------------------
  SCR_EntityCatalogEntry GetEntryWithPrefabFromFactionCatalog(EEntityCatalogType catalogType, ResourceName prefabToFind, notnull SCR_Faction faction)
     {
         //~ No prefab given
         if (SCR_StringHelper.IsEmptyOrWhiteSpace(prefabToFind))
             return null;

         //~ Get catalog
         SCR_EntityCatalog catalog = GetFactionEntityCatalogOfType(catalogType, faction);
         if (!catalog)
             return null;

         //~ Try and find prefab
         return catalog.GetEntryWithPrefab(prefabToFind);
     }

     //------------------------------------------------------------------------------------------------
  SCR_EntityCatalogEntry GetEntryWithPrefabFromGeneralOrFactionCatalog(EEntityCatalogType catalogType, ResourceName prefabToFind, notnull SCR_Faction faction, bool prioritizeGeneralCatalog = false)
     {
         //~ No prefab given
         if (SCR_StringHelper.IsEmptyOrWhiteSpace(prefabToFind))
             return null;

         SCR_EntityCatalogEntry foundEntry;

         //~ Search in faction first
         if (!prioritizeGeneralCatalog)
         {
             //~ Search in faction
             foundEntry = GetEntryWithPrefabFromFactionCatalog(catalogType, prefabToFind, faction);
             if (foundEntry)
                 return foundEntry;

             //~ Search in general
             foundEntry = GetEntryWithPrefabFromCatalog(catalogType, prefabToFind);
             if (foundEntry)
                 return foundEntry;
         }
         //~ Search in general catalog first
         else
         {
             //~ Search in faction
             foundEntry = GetEntryWithPrefabFromCatalog(catalogType, prefabToFind);
             if (foundEntry)
                 return foundEntry;

             //~ Search in general
             foundEntry = GetEntryWithPrefabFromFactionCatalog(catalogType, prefabToFind, faction);
             if (foundEntry)
                 return foundEntry;
         }

         //~ Was not found
         return null;
     }

     //------------------------------------------------------------------------------------------------
  SCR_EntityCatalogEntry GetEntryWithPrefabFromAnyCatalog(EEntityCatalogType catalogType, ResourceName prefabToFind, SCR_Faction priorityFaction = null, bool prioritizeGeneralOfPriorityFaction = false)
     {
         SCR_EntityCatalogEntry foundEntry;

         if (!prioritizeGeneralOfPriorityFaction)
         {
             //~ Search in priority faction first
             if (priorityFaction)
             {
                 foundEntry = GetEntryWithPrefabFromFactionCatalog(catalogType, prefabToFind, priorityFaction);
                 if (foundEntry)
                     return foundEntry;
             }

             //~ Search in general
             foundEntry = GetEntryWithPrefabFromCatalog(catalogType, prefabToFind);
             if (foundEntry)
                 return foundEntry;
         }
         else
         {
             //~ Search in general first
             foundEntry = GetEntryWithPrefabFromCatalog(catalogType, prefabToFind);
             if (foundEntry)
                 return foundEntry;

             //~ Search in priority faction
             if (priorityFaction)
             {
                 foundEntry = GetEntryWithPrefabFromFactionCatalog(catalogType, prefabToFind, priorityFaction);
                 if (foundEntry)
                     return foundEntry;
             }
         }

         if (!m_FactionManager)
             return null;

         //~ Search all faction (ignoring priorityFaction) and try to find the prefab
         array<Faction> factions = {};
         m_FactionManager.GetFactionsList(factions);

         foreach (Faction faction : factions)
         {
             SCR_Faction scrFaction = SCR_Faction.Cast(faction);

             //~ SCR_Faction not found or already searched
             if (!scrFaction || scrFaction == priorityFaction)
                 continue;

             //~ Try and find the prefab
             foundEntry = GetEntryWithPrefabFromFactionCatalog(catalogType, prefabToFind, scrFaction);
             if (foundEntry)
                 return foundEntry;
         }

         return null;
     }

     //======================================== SPAWNER ========================================\\
         //--------------------------------- Get List of valid prefabs for spawner ---------------------------------\\

  int GetFilteredEditorPrefabsOfAllFactions(EEntityCatalogType catalogType, EEditorMode editorMode, notnull out array<ResourceName> filteredPrefabsList, array<EEditableEntityLabel> includedLabels = null, array<EEditableEntityLabel> excludedLabels = null, bool needsAllIncludedLabels = true, bool getFactionLessPrefabs = false)
     {
         filteredPrefabsList.Clear();

         array<Faction> factions = {};

         //~ Get sorted factions
         SCR_DelegateFactionManagerComponent delegateFactionManager = SCR_DelegateFactionManagerComponent.GetInstance();
         if (delegateFactionManager)
         {
             SCR_SortedArray<SCR_EditableFactionComponent> sortedDelegates = new SCR_SortedArray<SCR_EditableFactionComponent>();
             int count = delegateFactionManager.GetSortedFactionDelegates(sortedDelegates);

             //~ Get sorted factions
             for(int i = 0; i < count; ++i)
             {
                 factions.Insert(sortedDelegates.Get(i).GetFaction());
             }

         }
         //~ No delegate found so get factions from faction manager
         else if (m_FactionManager)
         {
             m_FactionManager.GetFactionsList(factions);
         }
         //~ Could not find faction nor faction manager
         else
         {
             return 0;
         }

         array<ResourceName> filteredPrefabsOfFaction = {};
         SCR_Faction scrFaction;

         //~ Get filtered prefabs of each faction
         foreach (Faction faction : factions)
         {
             scrFaction = SCR_Faction.Cast(faction);
             if (!scrFaction)
                 continue;

             GetFilteredEditorPrefabs(catalogType, editorMode, scrFaction, filteredPrefabsOfFaction, includedLabels, excludedLabels, needsAllIncludedLabels);
             filteredPrefabsList.InsertAll(filteredPrefabsOfFaction);
         }

         //~ Get filtered prefabs of factionless entries
         if (getFactionLessPrefabs)
         {
             GetFilteredEditorPrefabs(catalogType, editorMode, null, filteredPrefabsOfFaction, includedLabels, excludedLabels, needsAllIncludedLabels);
             filteredPrefabsList.InsertAll(filteredPrefabsOfFaction);
         }

         return filteredPrefabsList.Count();
     }

     //--------------------------------- Get List of valid prefabs for spawner ---------------------------------\\

  int GetFilteredEditorPrefabs(EEntityCatalogType catalogType, EEditorMode editorMode, SCR_Faction faction, notnull out array<ResourceName> filteredPrefabsList, array<EEditableEntityLabel> includedLabels = null, array<EEditableEntityLabel> excludedLabels = null, bool needsAllIncludedLabels = true)
     {
         SCR_EntityCatalog catalog;
         filteredPrefabsList.Clear();

         //~ Get the catalog from faction or factionless
         if (faction)
             catalog = GetFactionEntityCatalogOfType(catalogType, faction);
         else
             catalog = GetEntityCatalogOfType(catalogType);

         //~ No catalog found
         if (!catalog)
             return 0;

         array<SCR_EntityCatalogEntry> filteredEntityList = {};
         array<typename> filterClassArray = {};
         filterClassArray.Insert(SCR_EntityCatalogEditorData);

         catalog.GetFullFilteredEntityList(filteredEntityList, includedLabels, excludedLabels, filterClassArray, needsAllIncludedLabels: needsAllIncludedLabels);

         SCR_EntityCatalogEditorData editorData;

         //~ Get all valid prefabs for mode
         foreach (SCR_EntityCatalogEntry entry : filteredEntityList)
         {
             editorData = SCR_EntityCatalogEditorData.Cast(entry.GetEntityDataOfType(SCR_EntityCatalogEditorData));

             //~ Ignore entries not valid in current editor mode
             if (!editorData.IsValidInEditorMode(editorMode))
                 continue;

             //~ Add the prefab to the array
             filteredPrefabsList.Insert(entry.GetPrefab());
         }

         return filteredPrefabsList.Count();
     }

     //======================================== ARSENAL ========================================\\

     //------------------------------------------------------------------------------------------------
  bool GetArsenalItems(out array<SCR_ArsenalItem> arsenalItems, SCR_EArsenalItemType typeFilter = -1, SCR_EArsenalItemMode modeFilter = -1, EArsenalItemDisplayType requiresDisplayType = -1)
     {
         arsenalItems.Clear();

         SCR_EntityCatalog itemCatalog = GetEntityCatalogOfType(EEntityCatalogType.ITEM);
         if (!itemCatalog)
             return false;

         return GetArsenalItems(arsenalItems, itemCatalog, typeFilter, modeFilter, requiresDisplayType);
     }

     //------------------------------------------------------------------------------------------------
  bool GetFactionArsenalItems(out array<SCR_ArsenalItem> arsenalItems, SCR_Faction faction, SCR_EArsenalItemType typeFilter = -1, SCR_EArsenalItemMode modeFilter = -1, EArsenalItemDisplayType requiresDisplayType = -1)
     {
         arsenalItems.Clear();

         SCR_EntityCatalog itemCatalog = GetFactionEntityCatalogOfType(EEntityCatalogType.ITEM, faction);
         if (!itemCatalog)
             return false;

         return GetArsenalItems(arsenalItems, itemCatalog, typeFilter, modeFilter, requiresDisplayType);
     }

     //------------------------------------------------------------------------------------------------
  protected bool GetArsenalItems(out array<SCR_ArsenalItem> arsenalItems, notnull SCR_EntityCatalog itemCatalog, SCR_EArsenalItemType typeFilter = -1, SCR_EArsenalItemMode modeFilter = -1, EArsenalItemDisplayType requiresDisplayType = -1)
     {
         array<SCR_EntityCatalogEntry> arsenalEntries = {};
         array<SCR_BaseEntityCatalogData> arsenalDataList = {};
         itemCatalog.GetEntityListWithData(SCR_ArsenalItem, arsenalEntries, arsenalDataList);

         SCR_ArsenalItem arsenalItem;

         foreach (SCR_BaseEntityCatalogData arsenalData: arsenalDataList)
         {
             //~ Get Arsenal data
             arsenalItem = SCR_ArsenalItem.Cast(arsenalData);

             //~ Does not have type so skip
             if (typeFilter != -1 && !SCR_Enum.HasPartialFlag(arsenalItem.GetItemType(), typeFilter))
                 continue;

             //~ Does not have mode so skip
             if (modeFilter != -1 && !SCR_Enum.HasPartialFlag(arsenalItem.GetItemMode(), modeFilter))
                 continue;

             //~ Get if has display data, ignore if it doesn't
             if (requiresDisplayType != -1 && !arsenalItem.GetDisplayDataOfType(requiresDisplayType))
                 continue;

             arsenalItems.Insert(arsenalItem);
         }

         return !arsenalItems.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  int GetAllArsenalItems(out array<SCR_ArsenalItem> allArsenalItems, SCR_EArsenalItemType typeFilter = -1, SCR_EArsenalItemMode modeFilter = -1, EArsenalItemDisplayType requiresDisplayType = -1)
     {
         allArsenalItems.Clear();

         array<Faction> factions = {};
         m_FactionManager.GetFactionsList(factions);
         SCR_Faction scrFaction;
         array<SCR_ArsenalItem> arsenalItems = {};

         foreach (Faction faction : factions)
         {
             scrFaction = SCR_Faction.Cast(faction);
             if (!scrFaction)
                 continue;

             GetFactionArsenalItems(arsenalItems, scrFaction, typeFilter, modeFilter, requiresDisplayType);
             allArsenalItems.InsertAll(arsenalItems);
         }

         GetArsenalItems(arsenalItems, typeFilter, modeFilter, requiresDisplayType);
         allArsenalItems.InsertAll(arsenalItems);

         return allArsenalItems.Count();
     }

     //------------------------------------------------------------------------------------------------
     array<SCR_ArsenalItem> GetFilteredArsenalItems(SCR_EArsenalItemType typeFilter, SCR_EArsenalItemMode modeFilter, SCR_Faction faction = null, EArsenalItemDisplayType requiresDisplayType = -1)
     {
         array<SCR_ArsenalItem> refFilteredItems = {};
         array<SCR_ArsenalItem> filteredItems = {};

         if (faction)
             GetFactionArsenalItems(refFilteredItems, faction, typeFilter, modeFilter, requiresDisplayType);
         else
             GetArsenalItems(refFilteredItems, typeFilter, modeFilter, requiresDisplayType);

         foreach (SCR_ArsenalItem item : refFilteredItems)
         {
             filteredItems.Insert(item);
         }

         return filteredItems;
     }

     //======================================== INIT ========================================\\

     //------------------------------------------------------------------------------------------------
  static void InitCatalogs(notnull array<ref SCR_EntityCatalog> entityCatalogArray, notnull map<EEntityCatalogType, ref SCR_EntityCatalog> entityCatalogMap)
     {
         SCR_EntityCatalog foundCatalog;

         //~ Move catalogs to map for quicker processing
         foreach (SCR_EntityCatalog entityCatalog : entityCatalogArray)
         {
             //~ Catalog not part of map so add it
             if (!entityCatalogMap.Find(entityCatalog.GetCatalogType(), foundCatalog))
                 entityCatalogMap.Insert(entityCatalog.GetCatalogType(), entityCatalog);
             //~ Catalog is part of map so merge them
             else
                 foundCatalog.MergeCatalogs(entityCatalog);
         }
     }

     //------------------------------------------------------------------------------------------------
     override void EOnInit(IEntity owner)
     {
         m_FactionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
         if (!m_FactionManager)
             Debug.Error2("SCR_EntityCatalogManagerComponent", "Could not find SCR_FactionManager, this is required for many the Getter Functions!");

         //~ Init the catalog
         InitCatalogs(m_aEntityCatalogs, m_mEntityCatalogs);

         m_bInitDone = true;

         //~ Clear array as no longer needed
         m_aEntityCatalogs = null;
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         if (SCR_Global.IsEditMode())
             return;

         if (s_Instance)
         {
             Print("More than one 'SCR_EntityCatalogManagerComponent' excist in the world! Make sure there is only 1!", LogLevel.ERROR);
             return;
         }

         //~ Set instance
         s_Instance = this;

         SetEventMask(owner, EntityEvent.INIT);
     }
 }