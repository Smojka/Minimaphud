 //------------------------------------------------------------------------------------------------
 enum SCR_EMapMarkerType
 {
     UNSET = 0,
     SIMPLE,             // simple static marker entry
     DYNAMIC_EXAMPLE,    // example entry of a dynamic marker
     PLACED_CUSTOM,      // configurable placed static marker
     PLACED_MILITARY,    // placed static marker - predefined military symbol
     SQUAD_LEADER // dynamic squad leader marker
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(configRoot: true)]
 class SCR_MapMarkerConfig
 {
     [Attribute("", UIWidgets.Object, "Definition of map marker types")]
     protected ref array<ref SCR_MapMarkerEntryConfig> m_aMarkerEntryConfigs;

     //------------------------------------------------------------------------------------------------
     array<ref SCR_MapMarkerEntryConfig> GetMarkerEntryConfigs()
     {
         return m_aMarkerEntryConfigs;
     }

     //------------------------------------------------------------------------------------------------
  SCR_MapMarkerEntryConfig GetMarkerEntryConfigByType(SCR_EMapMarkerType type)
     {
         if (!m_aMarkerEntryConfigs || m_aMarkerEntryConfigs.IsEmpty())
             return null;

         foreach (SCR_MapMarkerEntryConfig cfg : m_aMarkerEntryConfigs)
         {
             if (cfg.GetMarkerType() == type)
                 return cfg;
         }

         return null;
     }
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(), SCR_MapMarkerTitle()]
 class SCR_MapMarkerEntryConfig
 {
     [Attribute("{DD15734EB89D74E2}UI/layouts/Map/MapMarkerBase.layout", UIWidgets.ResourceNamePicker, desc: "Marker layout", params: "layout")]
     protected ResourceName m_sMarkerLayout;

     //------------------------------------------------------------------------------------------------
  SCR_EMapMarkerType GetMarkerType()
     {
         return SCR_EMapMarkerType.UNSET;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetMarkerLayout()
     {
         return m_sMarkerLayout;
     }

     //------------------------------------------------------------------------------------------------
  void InitClientSettings(SCR_MapMarkerBase marker, SCR_MapMarkerWidgetComponent widgetComp)
     {
         int ownerID = marker.GetMarkerOwnerID();

         if (ownerID > 0)
         {
             if (GetGame().GetPlayerController().GetPlayerId() == ownerID)
                 widgetComp.SetModeIcon(true, marker.GetMarkerID() != -1);
             else
                 widgetComp.SetAuthor("(" + GetGame().GetPlayerManager().GetPlayerName(ownerID) + ")");

         }
     }

     //------------------------------------------------------------------------------------------------
  void InitClientSettingsDynamic(notnull SCR_MapMarkerEntity marker, notnull SCR_MapMarkerDynamicWComponent widgetComp);

     //------------------------------------------------------------------------------------------------
  void InitServerLogic();

     //------------------------------------------------------------------------------------------------
  void InitClientLogic();

     //------------------------------------------------------------------------------------------------
  void OnMapLayerChanged(notnull SCR_MapMarkerWidgetComponent widgetComp, int layerID);

     //------------------------------------------------------------------------------------------------
  void OnMapLayerChangedDynamic(notnull SCR_MapMarkerDynamicWComponent widgetComp, int layerID);

     //------------------------------------------------------------------------------------------------
  void OnMapInit(notnull SCR_MapEntity mapEnt, notnull SCR_MapMarkersUI markerUIComp);

     //------------------------------------------------------------------------------------------------
  void OnMapOpen(notnull SCR_MapEntity mapEnt, notnull SCR_MapMarkersUI markerUIComp);

     //------------------------------------------------------------------------------------------------
  void OnMapClose(notnull SCR_MapEntity mapEnt, notnull SCR_MapMarkersUI markerUIComp);
 }