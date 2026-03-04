
 class SCR_AIFireteamsClusterActivity : SCR_AIFireteamsActivity
 {
     protected SCR_AITargetClusterState m_ClusterState;

     //------------------------------------------------------------------------------------
     void SCR_AIFireteamsClusterActivity(SCR_AIGroupUtilityComponent utility, AIWaypoint relatedWaypoint, notnull SCR_AITargetClusterState clusterState)
     {
         m_ClusterState = clusterState;

         utility.m_Perception.GetOnTargetClusterDeleted().Insert(Event_OnTargetClusterStateDeleted);
     }

     //------------------------------------------------------------------------------------
     void ~SCR_AIFireteamsClusterActivity()
     {
         if (m_Utility)
             m_Utility.m_Perception.GetOnTargetClusterDeleted().Remove(Event_OnTargetClusterStateDeleted);
     }

     //------------------------------------------------------------------------------------
     void Event_OnTargetClusterStateDeleted(SCR_AITargetClusterState s)
     {
         if (s != m_ClusterState)
             return;

         #ifdef AI_DEBUG
         AddDebugMessage("Target cluster doesn't exist any more, activity is complete");
         #endif

         // Group either forgot about it, or it was destroyed
         Complete();
     }
 }