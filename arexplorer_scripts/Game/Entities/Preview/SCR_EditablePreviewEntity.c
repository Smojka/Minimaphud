 [EntityEditorProps(category: "GameScripted/Preview", color: "0 0 0 0")]
 class SCR_EditablePreviewEntityClass: SCR_GenericPreviewEntityClass
 {
 };
 class SCR_EditablePreviewEntity: SCR_GenericPreviewEntity
 {
     protected SCR_EditableEntityComponent m_EditableEntity;
     protected ref array<IEntity> m_aExcludeArray; //--- Entities excluded when tracing terrain. Must be here, trace does not hold own reference.

     static SCR_BasePreviewEntity SpawnPreviewFromEditableOwner(IEntity entity, ResourceName previewPrefab, BaseWorld world = null, EntitySpawnParams spawnParams = null, ResourceName material = ResourceName.Empty, EPreviewEntityFlag flags = 0)
     {
         SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.GetEditableEntity(entity);
         if (editableEntity)
             return SpawnPreviewFromEditableEntity(editableEntity, previewPrefab, world, spawnParams, material, flags);
         else
             return null;
     }
     static SCR_BasePreviewEntity SpawnPreviewFromEditableEntity(SCR_EditableEntityComponent entity, ResourceName previewPrefab, BaseWorld world = null, EntitySpawnParams spawnParams = null, ResourceName material = ResourceName.Empty, EPreviewEntityFlag flags = 0)
     {
         if (!entity)
         {
             Print("No entity defined", LogLevel.WARNING);
             return null;
         }
         array<ref SCR_BasePreviewEntry> entries = GetPreviewEntriesFromEditableEntity(entity, spawnParams, flags);
         return SpawnPreview(entries, previewPrefab, world, spawnParams, material, flags);
     }
     static SCR_BasePreviewEntity SpawnPreviewFromEditableEntities(notnull set<SCR_EditableEntityComponent> entities, ResourceName previewPrefab, BaseWorld world = null, EntitySpawnParams spawnParams = null, ResourceName material = ResourceName.Empty, EPreviewEntityFlag flags = 0)
     {
         if (!entities || entities.IsEmpty())
         {
             Print("No editable entities defined", LogLevel.WARNING);
             return null;
         }

         EntitySpawnParams spawnParamsLocal = spawnParams;
         if (!spawnParamsLocal)
             spawnParamsLocal = new EntitySpawnParams();

         TraceParam trace;
         array<IEntity> excludeArray = {};
         if (SCR_Enum.HasFlag(flags, EPreviewEntityFlag.GEOMETRY))
             trace = new TraceParam();

         array<ref SCR_BasePreviewEntry> entries = {};
         set<SCR_EditableEntityComponent> entitiesCopy = new set<SCR_EditableEntityComponent>;
         entitiesCopy.Copy(entities);
         int index;
         while (!entitiesCopy.IsEmpty())
         {
             SCR_EditableEntityComponent entity = entitiesCopy[index];
             SCR_EditableEntityComponent entityParent = entity.GetParentEntity();
             if (entityParent && entitiesCopy.Find(entityParent) != -1)
             {
                 //--- Parent is edited, but not registered yet; skip
                 index++;
                 continue;
             }
             else
             {
                 GetPreviewEntries(entity, entries, spawnParamsLocal.Transform, -1, flags, trace);
                 entitiesCopy.Remove(index);
                 index = 0;
             }
         }
         return SpawnPreview(entries, previewPrefab, world, spawnParamsLocal, material, flags);
     }

     static array<ref SCR_BasePreviewEntry> GetPreviewEntriesFromEditableEntity(SCR_EditableEntityComponent entity, out EntitySpawnParams spawnParams = null, EPreviewEntityFlag flags = 0)
     {
         if (!spawnParams)
             spawnParams = new EntitySpawnParams();

         TraceParam trace;
         if (SCR_Enum.HasFlag(flags, EPreviewEntityFlag.GEOMETRY))
             trace = new TraceParam();

         array<ref SCR_BasePreviewEntry> entries = {};
         GetPreviewEntries(entity, entries, spawnParams.Transform, -1, flags, trace);
         return entries;
     }

     static void GetPreviewEntries(SCR_EditableEntityComponent entity, out notnull array<ref SCR_BasePreviewEntry> outEntries, out vector rootTransform[4], int parentID = -1, EPreviewEntityFlag flags = 0, TraceParam trace = null)
     {
         IEntity owner = entity.GetOwner();
         vector transform[4];
         if (parentID == -1)
         {
             entity.GetTransform(transform);
             SaveRootTransform(transform, rootTransform);
         }
         else
         {
             //--- Skip entities flagged by IGNORE_LAYERS when their patrent is edited as well (e.g., don't move waypoints when moving a group)
             if (entity.HasEntityFlag(EEditableEntityFlag.IGNORE_LAYERS))
                 return;

             //--- Skip the entity if...
             if (
                 //--- ... it has an object (e.g., not a composition folder)...
                 owner.GetVObject()
                 &&
                 (
                     //--- ... but the object does not have a mesh (e.g., particle effect)...
                     (!owner.GetVObject().ToMeshObject())
                     ||
                     //--- ... or the object is not visible or is not intended for play mode
                     (!(owner.GetFlags() & EntityFlags.VISIBLE) || (owner.GetFlags() & EntityFlags.EDITOR_ONLY))
                 )
             )
                 return;

             entity.GetLocalTransform(transform);
         }

         SCR_BasePreviewEntry entry = new SCR_BasePreviewEntry(true);
         entry.m_iParentID = parentID;
         parentID = outEntries.Insert(entry);

         entry.m_Entity = owner;
         entry.SaveTransform(transform);
         entry.SetScale(GetLocalScale(entry.m_Entity));
         entry.m_iPivotID = GetPivotName(entry.m_Entity);

         if (GetMesh(entry.m_Entity, flags, entry, outEntries))
             return;

         entry.m_Flags |= EPreviewEntityFlag.EDITABLE;

         if (entity.HasEntityFlag(EEditableEntityFlag.HORIZONTAL))
             entry.m_Flags |= EPreviewEntityFlag.HORIZONTAL;

         if (entity.HasEntityFlag(EEditableEntityFlag.ORIENT_CHILDREN))
             entry.m_Flags |= EPreviewEntityFlag.ORIENT_CHILDREN;

         if (!SCR_Enum.HasFlag(flags, EPreviewEntityFlag.IGNORE_TERRAIN))
         {
             bool isUnderwater = SCR_Enum.HasFlag(flags, EPreviewEntityFlag.UNDERWATER);
             SaveTerrainTransform(entry.m_Entity, entry, isUnderwater, trace);
         }
         else
         {
             //--- Don't calculate terrain under entity, but use default values. Without it, entities with modified pitch or bank would have them reset.
             entry.m_vAnglesTerrain = Vector(0, entry.m_vAngles[1], entry.m_vAngles[2]);
             entry.m_vHeightTerrain = entry.m_vPosition[1];
         }

         if (entry.m_Shape != EPreviewEntityShape.PREFAB)
         {
             //--- Scan children
             set<IEntity> childOwners = new set<IEntity>();

             //--- In editor hierarchy (ignore when children are shown individually, e.g., soldiers in a group, to prevent duplicate previews)
             if (!entity.HasEntityFlag(EEditableEntityFlag.INDIVIDUAL_CHILDREN) || SCR_Enum.HasFlag(flags, EPreviewEntityFlag.IGNORE_TERRAIN))
             {
                 set<SCR_EditableEntityComponent> children = entity.GetChildrenRef();
                 if (children)
                 {
                     for (int i = 0, count = children.Count(); i < count; i++)
                     {
                         GetPreviewEntries(children[i], outEntries, rootTransform, parentID, flags);
                         childOwners.Insert(children[i].GetOwner());
                     }
                 }
             }

             //--- In engine hierarchy
             if (!SCR_Enum.HasFlag(flags, EPreviewEntityFlag.ONLY_EDITABLE))
             {
                 IEntity child = entry.m_Entity.GetChildren();
                 while (child)
                 {
                     if (!childOwners.Contains(child)) //--- Check for duplicates with editor hierarchy
                         GetPreviewEntries(child, outEntries, rootTransform, parentID, flags);

                     child = child.GetSibling();
                 }
             }
         }
     }

     SCR_EditableEntityComponent GetEditableEntity()
     {
         return m_EditableEntity;
     }
     array<IEntity> GetExcludeArray()
     {
         return m_aExcludeArray;
     }

     protected override void EOnPreviewInit(SCR_BasePreviewEntry entry, SCR_BasePreviewEntity root)
     {
         super.EOnPreviewInit(entry, root);
         m_EditableEntity = SCR_EditableEntityComponent.GetEditableEntity(entry.m_Entity);

         if (entry.m_iParentID == -1 || (SCR_Enum.HasFlag(m_Flags, EPreviewEntityFlag.REGISTER_EDITABLE)))
         {
             //--- Let the preview copy appearance of source's editable entity
             SCR_EditablePreviewComponent previewEditableEntity = SCR_EditablePreviewComponent.Cast(FindComponent(SCR_EditablePreviewComponent));
             if (previewEditableEntity)
             {
                 if (m_EditableEntity)
                     previewEditableEntity.InitFromEntity(m_EditableEntity);
                 else
                     previewEditableEntity.InitFromSource(SCR_BaseContainerTools.FindComponentSource(entry.m_EntitySource, SCR_EditableEntityComponent));
             }
         }
     }
     override protected void EOnRootPreviewInit(array<ref SCR_BasePreviewEntry> entries)
     {
         m_aExcludeArray = {};
         foreach (SCR_BasePreviewEntry entry: entries)
         {
             m_aExcludeArray.Insert(entry.m_Entity);
         }
     }
 };