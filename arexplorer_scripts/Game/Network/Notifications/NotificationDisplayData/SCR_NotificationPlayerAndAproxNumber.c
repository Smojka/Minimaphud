
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationPlayerAndAproxNumber : SCR_NotificationAproxNumber
 {
     //------------------------------------------------------------------------------------------------
     override string GetText(SCR_NotificationData data)
     {
         int playerID;
         data.GetParams(playerID);

         string playerName;
         data.GetNotificationTextEntries(playerName);
         if (!GetPlayerName(playerID, playerName))
             return string.Empty;

         data.SetNotificationTextEntries(playerName);

         return super.GetText(data);
     }
 }