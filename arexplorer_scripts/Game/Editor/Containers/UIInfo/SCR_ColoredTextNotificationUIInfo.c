 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_ColoredTextNotificationUIInfo : SCR_UINotificationInfo
 {
     [Attribute(ENotificationColor.NEUTRAL.ToString(), UIWidgets.ComboBox, "Notification Text Color", "", ParamEnumArray.FromEnum(ENotificationColor))]
     protected ENotificationColor m_iNotificationTextColor;

     //------------------------------------------------------------------------------------------------
  ENotificationColor GetNotificationTextColor()
     {
         return m_iNotificationTextColor;
     }
 }