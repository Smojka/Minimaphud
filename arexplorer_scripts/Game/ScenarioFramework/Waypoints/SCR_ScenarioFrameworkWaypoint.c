 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_ScenarioFrameworkWaypoint
 {
     //TODO: Request Autocomplete attribute to be exposed to script with getters and setters
     //TODO: Request AI Behaviour Tree attribute to be exposed to script with getters and setters
     //TODO: Request AI Behaviour Tree Move To attribute to be exposed to script with getters and setters


     SCR_ScenarioFrameworkLayerBase m_SlotWaypoint;

     //------------------------------------------------------------------------------------------------
  void SetupWaypoint(IEntity waypointEntity)
     {
         AIWaypoint waypoint = AIWaypoint.Cast(waypointEntity);
         if (!waypoint)
             return;

         waypoint.SetCompletionRadius(GetWaypointCompletionRadius());
         waypoint.SetCompletionType(GetWaypointCompletionType());
     }

     //------------------------------------------------------------------------------------------------
  void SetWaypointCompletionRadius(float radius)
     {
     }

     //------------------------------------------------------------------------------------------------
  float GetWaypointCompletionRadius()
     {
         return 10;
     }

     //------------------------------------------------------------------------------------------------
  void SetWaypointCompletionType(EAIWaypointCompletionType type)
     {
     }

     //------------------------------------------------------------------------------------------------
     EAIWaypointCompletionType GetWaypointCompletionType()
     {
         return EAIWaypointCompletionType.Any;
     }

     //------------------------------------------------------------------------------------------------
  void SetWaypointPrefab(ResourceName prefab)
     {
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetWaypointPrefab()
     {
         return string.Empty;
     }
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_ScenarioFrameworkWaypointScripted : SCR_ScenarioFrameworkWaypoint
 {
     //------------------------------------------------------------------------------------------------
     override void SetupWaypoint(IEntity waypointEntity)
     {
         super.SetupWaypoint(waypointEntity);

         SCR_AIWaypoint waypointScripted = SCR_AIWaypoint.Cast(waypointEntity);
         if (!waypointScripted)
             return;

         waypointScripted.SetPriorityLevel(GetWaypointPriorityLevel());
     }

     //------------------------------------------------------------------------------------------------
  void SetWaypointPriorityLevel(float priority)
     {
     }

     //------------------------------------------------------------------------------------------------
  float GetWaypointPriorityLevel()
     {
         return 0;
     }
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_ScenarioFrameworkWaypointSmartAction : SCR_ScenarioFrameworkWaypointScripted
 {
     [Attribute("", UIWidgets.EditBox, "Static reference to entity with smart action")]
     string m_sStaticEntityName;

     //------------------------------------------------------------------------------------------------
     override void SetupWaypoint(IEntity waypointEntity)
     {
         super.SetupWaypoint(waypointEntity);

         SCR_SmartActionWaypoint waypointSmartAction = SCR_SmartActionWaypoint.Cast(waypointEntity);
         if (!waypointSmartAction)
             return;

         waypointSmartAction.m_sSmartActionTag = GetWaypointSmartActionTag();
         waypointSmartAction.m_sStaticEntityName = m_sStaticEntityName;
     }

     //------------------------------------------------------------------------------------------------
  void SetWaypointSmartActionTag(string tag)
     {
     }

     //------------------------------------------------------------------------------------------------
  string GetWaypointSmartActionTag()
     {
         return string.Empty;
     }
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_ScenarioFrameworkWaypointEntity : SCR_ScenarioFrameworkWaypointScripted
 {
     [Attribute("", UIWidgets.EditBox, "Related entity")]
     string  m_sEntityName;

     //------------------------------------------------------------------------------------------------
     override void SetupWaypoint(IEntity waypointEntity)
     {
         super.SetupWaypoint(waypointEntity);

         SCR_EntityWaypoint waypointScriptedEntity = SCR_EntityWaypoint.Cast(waypointEntity);
         if (!waypointScriptedEntity)
             return;

         waypointScriptedEntity.SetEntityName(m_sEntityName);
     }
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_ScenarioFrameworkWaypointTimed : SCR_ScenarioFrameworkWaypointScripted
 {
     //------------------------------------------------------------------------------------------------
     override void SetupWaypoint(IEntity waypointEntity)
     {
         super.SetupWaypoint(waypointEntity);

         SCR_TimedWaypoint waypointTimed = SCR_TimedWaypoint.Cast(waypointEntity);
         if (!waypointTimed)
             return;

         waypointTimed.SetHoldingTime(GetWaypointHoldingTime());
     }

     //------------------------------------------------------------------------------------------------
  void SetWaypointHoldingTime(float time)
     {
     }

     //------------------------------------------------------------------------------------------------
  float GetWaypointHoldingTime()
     {
         return 0;
     }
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_ScenarioFrameworkWaypointTimedDefend : SCR_ScenarioFrameworkWaypointTimed
 {
     //------------------------------------------------------------------------------------------------
     override void SetupWaypoint(IEntity waypointEntity)
     {
         super.SetupWaypoint(waypointEntity);

         SCR_DefendWaypoint waypointDefend = SCR_DefendWaypoint.Cast(waypointEntity);
         if (!waypointDefend)
             return;

         waypointDefend.SetFastInit(GetFastInit());
     }

     //------------------------------------------------------------------------------------------------
  void SetFastInit(bool enabled)
     {
     }

     //------------------------------------------------------------------------------------------------
  bool GetFastInit()
     {
         return false;
     }
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_ScenarioFrameworkWaypointBoarding : SCR_ScenarioFrameworkWaypointTimed
 {
     //------------------------------------------------------------------------------------------------
     override void SetupWaypoint(IEntity waypointEntity)
     {
         super.SetupWaypoint(waypointEntity);

         SCR_BoardingWaypoint waypointBoarding = SCR_BoardingWaypoint.Cast(waypointEntity);
         if (!waypointBoarding)
             return;

         waypointBoarding.SetAllowance(GetDriverAllowed(), GetGunnerAllowed(), GetCargoAllowed());
     }

     //------------------------------------------------------------------------------------------------
  void SetDriverAllowed(bool enabled)
     {
     }

     //------------------------------------------------------------------------------------------------
  bool GetDriverAllowed()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  void SetGunnerAllowed(bool enabled)
     {
     }

     //------------------------------------------------------------------------------------------------
  bool GetGunnerAllowed()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  void SetCargoAllowed(bool enabled)
     {
     }

     //------------------------------------------------------------------------------------------------
  bool GetCargoAllowed()
     {
         return false;
     }
 }