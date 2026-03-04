
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationGroupName : SCR_NotificationDisplayData
 {
     override string GetText(SCR_NotificationData data)
     {
         int groupID;
         data.GetParams(groupID);

         string groupName;
         data.GetNotificationTextEntries(groupName);
         if (!GetGroupNameFromGroupID(groupID, groupName))
             return string.Empty;

         data.SetNotificationTextEntries(groupName);
         return super.GetText(data);
     }
 };