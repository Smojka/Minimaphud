
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationPlayerTargetFaction : SCR_NotificationDisplayData
 {

     override string GetText(SCR_NotificationData data)
     {
         int playerID, factionID;
         data.GetParams(playerID, factionID);

         string playerName, factionName;
         data.GetNotificationTextEntries(playerName, factionName);
         if (!GetPlayerName(playerID, playerName) || !GetFactionName(factionID, factionName))
             return string.Empty;

         data.SetNotificationTextEntries(playerName, factionName);
         return super.GetText(data);
     }

     //~ Sets position to first target
     override void SetPosition(SCR_NotificationData data)
     {
         if (!CanSetPosition(data))
             return;

         int playerID;
         data.GetParams(playerID);
         SetPositionDataEditablePlayer(playerID, data);
     }
 };