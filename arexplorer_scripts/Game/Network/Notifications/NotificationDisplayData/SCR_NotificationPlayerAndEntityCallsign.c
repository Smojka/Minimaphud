
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationPlayerAndEntityCallsign : SCR_NotificationPlayer
 {
     override string GetText(SCR_NotificationData data)
     {
         int playerID, entityID;
         data.GetParams(playerID, entityID);

         string playerName, callsign;
         data.GetNotificationTextEntries(playerName, callsign);
         if (!GetPlayerName(playerID, playerName) || !GetEntityCallsign(entityID, callsign))
             return string.Empty;

         data.SetNotificationTextEntries(playerName, callsign);
         return super.GetText(data);
     }
 };