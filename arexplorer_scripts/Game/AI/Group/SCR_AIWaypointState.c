
 class SCR_AIWaypointState : Managed
 {
     protected SCR_AIGroupUtilityComponent m_Utility;
     protected AIWaypoint m_Waypoint;

     void SCR_AIWaypointState(notnull SCR_AIGroupUtilityComponent utility, SCR_AIWaypoint waypoint)
     {
         m_Utility = utility;
         m_Waypoint = waypoint;
     }


     void OnSelected();

     void OnDeselected()
     {
         m_Utility.CancelActivitiesRelatedToWaypoint(m_Waypoint);
     }

     void OnExecuteWaypointTree()
     {
         m_Utility.CancelActivitiesRelatedToWaypoint(m_Waypoint, SCR_AIMoveActivity);
     }
 }