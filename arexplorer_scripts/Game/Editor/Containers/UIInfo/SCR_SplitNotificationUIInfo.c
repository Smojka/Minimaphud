 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_SplitNotificationUIInfo : SCR_UINotificationInfo
 {
     [Attribute(desc: "The right part of the split notification message. Only valid if m_bIsSplitNotification is true")]
     protected LocalizedString m_sSplitNotificationRightMessage;

     [Attribute("0", UIWidgets.ComboBox, "Left Notification text color", "", ParamEnumArray.FromEnum(ENotificationColor) )]
     protected ENotificationColor m_iLeftTextColor;

     [Attribute("0", UIWidgets.ComboBox, "Right Notification text color", "", ParamEnumArray.FromEnum(ENotificationColor) )]
     protected ENotificationColor m_iRightTextColor;

     [Attribute(desc: "This is for notifications that involve two entities and the two entities are friendlies. eg: Player Killed player. If Right (killed player) is friendly with Left (killer) then Left will be colored the same color. If there are only one or less entities involved this will be ignored (Supported notification classes: SCR_NotificationPlayerTargetPlayer, SCR_NotificationPlayerTargetEditableEntity, SCR_NotificationEditableEntityTargetPlayer and SCR_NotificationEditableEntityEditableEntityTarget)")]
     protected bool m_bReplaceLeftColorWithRightColorIfFactionsAreAlly;

     //------------------------------------------------------------------------------------------------
  string GetSplitRightMessageText()
     {
         return m_sSplitNotificationRightMessage;
     }

     //------------------------------------------------------------------------------------------------
  ENotificationColor GetLeftTextColor()
     {
         return m_iLeftTextColor;
     }

     //------------------------------------------------------------------------------------------------
  ENotificationColor GetRightTextColor()
     {
         return m_iRightTextColor;
     }

     //------------------------------------------------------------------------------------------------
  bool ShouldReplaceLeftColorWithRightColorIfAlly()
     {
         return m_bReplaceLeftColorWithRightColorIfFactionsAreAlly;
     }
 }