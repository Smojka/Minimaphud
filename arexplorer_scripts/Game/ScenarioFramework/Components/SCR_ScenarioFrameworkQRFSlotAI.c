 [EntityEditorProps(category: "GameScripted/ScenarioFramework/Slot", description: "")]
 class SCR_ScenarioFrameworkQRFSlotAIClass : SCR_ScenarioFrameworkSlotAIClass
 {
 }

 class SCR_ScenarioFrameworkQRFSlotAI : SCR_ScenarioFrameworkSlotAI
 {
     [Attribute(SCR_EQRFGroupType.INFANTRY.ToString(), UIWidgets.Flags, desc: "For which group types this spawn point is meant to be used", enums: ParamEnumArray.FromEnum(SCR_EQRFGroupType))]
     protected int m_iGroupType;

     [Attribute(desc: "List of containing distance that this spawn point requires in order to spawn QRF of given type - when spawn point allows for QRF type for which it doesnt have the safe distance then its considered good for use even when player is standing on that position")]
     protected ref array<ref SCR_QRFSpawnSafeZone> m_aSCR_QRFSpawnSafeZone;

     protected ref array<ref SCR_QRFWaypointConfig> m_aQRFWaypoints = {};

     //------------------------------------------------------------------------------------------------
  int GetGroupType()
     {
         return m_iGroupType;
     }

     //------------------------------------------------------------------------------------------------
     array<ref SCR_QRFSpawnSafeZone> GetSpawnSafeZones()
     {
         return m_aSCR_QRFSpawnSafeZone;
     }

     //------------------------------------------------------------------------------------------------
     void SetObjectToSpawn(ResourceName newObjectPrefab)
     {
         if (Resource.Load(newObjectPrefab).IsValid())
             m_sObjectToSpawn = newObjectPrefab;
     }

     //------------------------------------------------------------------------------------------------
  int GetNumberOfExistingWaypoints()
     {
         return m_aWaypoints.Count();
     }

     //------------------------------------------------------------------------------------------------
  void ClearWaypoints(bool deleteWaypointEntities = false)
     {
         if (deleteWaypointEntities)
         {
             for (int i = m_aWaypoints.Count() - 1; i >= 0; i --)
             {
                 SCR_EntityHelper.DeleteEntityAndChildren(m_aWaypoints[i]);
             }
         }
         m_aWaypoints.Clear();
         m_aQRFWaypoints.Clear();
     }

     //------------------------------------------------------------------------------------------------
     AIWaypoint CreateWaypoint(vector position, ResourceName wpPrefab)
     {
         AIWaypoint wp = SpawnWaypoint(position, wpPrefab);
         if (!wp)
             return null;

         m_aQRFWaypoints.Insert(new SCR_QRFWaypointConfig(wpPrefab, wpPosition : position));
         return wp;
     }

     //------------------------------------------------------------------------------------------------
  protected AIWaypoint SpawnWaypoint(vector position, ResourceName wpPrefab)
     {
         EntitySpawnParams paramsPatrolWP = new EntitySpawnParams();
         paramsPatrolWP.TransformMode = ETransformMode.WORLD;
         paramsPatrolWP.Transform[3] = position;

         Resource resWP = Resource.Load(wpPrefab);
         if (!resWP.IsValid())
             return null;

         AIWaypoint waypoint = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(resWP, GetGame().GetWorld(), paramsPatrolWP));
         if (!waypoint)
             return null;

         SCR_EntityHelper.SnapToGround(waypoint, onlyStatic : true);
         m_aWaypoints.Insert(waypoint);
         return waypoint;
     }

     //------------------------------------------------------------------------------------------------
  protected void ProcessQueuedWaypoints(SCR_ScenarioFrameworkLayerBase layer)
     {
         GetOnAllChildrenSpawned().Remove(ProcessWaypoints);
         if (m_aQRFWaypoints.IsEmpty() || m_aSpawnedEntities.IsEmpty())
             return;

         m_aWaypoints.Clear();
         foreach (SCR_QRFWaypointConfig wpConfig : m_aQRFWaypoints)
         {
             m_aWaypoints.Insert(SpawnWaypoint(wpConfig.GetPosition(), wpConfig.GetWaypointPrefabName()));
         }

         SCR_AIGroup group;
         foreach (IEntity entity : m_aSpawnedEntities)
         {
             group = SCR_AIGroup.Cast(entity);
             if (!group)
                 continue;

             foreach (AIWaypoint wp : m_aWaypoints)
             {
                 group.AddWaypoint(wp);
             }
         }
     }

     //------------------------------------------------------------------------------------------------
     override void Init(SCR_ScenarioFrameworkArea area = null, SCR_ScenarioFrameworkEActivationType activation = SCR_ScenarioFrameworkEActivationType.SAME_AS_PARENT)
     {
         if (m_bDynamicallyDespawned && !m_aQRFWaypoints.IsEmpty() && !SCR_StringHelper.IsEmptyOrWhiteSpace(m_sObjectToSpawn))
         {
             GetOnAllChildrenSpawned().Remove(ProcessQueuedWaypoints);
             GetOnAllChildrenSpawned().Insert(ProcessQueuedWaypoints);
         }
         super.Init(area, activation);
     }

     //------------------------------------------------------------------------------------------------
     protected override void SetWaypointToAI(SCR_ScenarioFrameworkLayerBase layer)
     {
         GetOnAllChildrenSpawned().Remove(SetWaypointToAI);
         if (!m_Entity || (!Resource.Load(m_sWPToSpawn).IsValid() && m_aSlotWaypoints.IsEmpty()))
             return;

         super.SetWaypointToAI(layer);
     }
 }