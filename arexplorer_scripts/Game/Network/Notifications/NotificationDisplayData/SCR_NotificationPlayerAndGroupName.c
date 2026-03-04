
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationPlayerAndGroupName : SCR_NotificationPlayer
 {
     override string GetText(SCR_NotificationData data)
     {
         int playerID, groupID;
         data.GetParams(playerID, groupID);

         string playerName, groupName;
         data.GetNotificationTextEntries(playerName, groupName);
         if (!GetPlayerName(playerID, playerName) || !GetGroupNameFromGroupID(groupID, groupName))
             return string.Empty;

         data.SetNotificationTextEntries(playerName, groupName);
         return super.GetText(data);
     }
 };