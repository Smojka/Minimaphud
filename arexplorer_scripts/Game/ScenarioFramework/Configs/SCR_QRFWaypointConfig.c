 enum SCR_EQRFGroupOrderType : SCR_EQRFGroupType
 {
     ANY = 1 << 16,
 }

 class SCR_QRFWaypointConfig : ScriptAndConfig
 {
     [Attribute(desc: "Waypoint prefab", "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et", category: "Waypoints")]
     protected ResourceName m_sWPToSpawn;

     [Attribute(SCR_EQRFGroupOrderType.ANY.ToString(), UIWidgets.ComboBox, desc: "To which group type this waypoint is meant to be applied to", "", ParamEnumArray.FromEnum(SCR_EQRFGroupOrderType))]
     protected SCR_EQRFGroupOrderType m_eGroupType;

     [Attribute("0", UIWidgets.Auto, "Offset that will be applied to the position of this WP in relation to QRF target location in direction of spawned group", "0 "+int.MAX+" 1")]
     protected int m_iDistanceOffsetToTargetLocation;

     protected vector m_vPosition;

     //------------------------------------------------------------------------------------------------
     ResourceName GetWaypointPrefabName()
     {
         return m_sWPToSpawn;
     }

     //------------------------------------------------------------------------------------------------
     SCR_EQRFGroupOrderType GetOrderType()
     {
         return m_eGroupType;
     }

     //------------------------------------------------------------------------------------------------
     int GetDistanceOffsetToTargetLocation()
     {
         return m_iDistanceOffsetToTargetLocation;
     }

     //------------------------------------------------------------------------------------------------
     vector GetPosition()
     {
         return m_vPosition;
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_QRFWaypointConfig(ResourceName wpPrefab, SCR_EQRFGroupOrderType groupType = SCR_EQRFGroupOrderType.ANY, int distanceOffset = 0, vector wpPosition = vector.Zero)
     {
         if (SCR_StringHelper.IsEmptyOrWhiteSpace(wpPrefab))
             return;

         m_sWPToSpawn = wpPrefab;
         m_eGroupType = groupType;
         m_iDistanceOffsetToTargetLocation = distanceOffset;
         m_vPosition = wpPosition;
     }
 }