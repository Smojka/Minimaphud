 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_UINotificationInfo : SCR_UIInfo
 {
     [Attribute("0", UIWidgets.ComboBox, "Notification Color", "", ParamEnumArray.FromEnum(ENotificationColor) )]
     protected ENotificationColor m_cNotificationColor;

     [Attribute("0", UIWidgets.ComboBox, "Editor Position Data", "", ParamEnumArray.FromEnum(ENotificationSetPositionData) )]
     protected ENotificationSetPositionData m_EditorSetPositionData;

     //------------------------------------------------------------------------------------------------
  ENotificationColor GetNotificationColor()
     {
         return m_cNotificationColor;
     }

     //------------------------------------------------------------------------------------------------
  ENotificationSetPositionData GetEditorSetPositionData()
     {
         return m_EditorSetPositionData;
     }
 }