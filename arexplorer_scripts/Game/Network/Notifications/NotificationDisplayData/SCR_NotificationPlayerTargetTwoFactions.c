
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationPlayerTargetTwoFactions : SCR_NotificationPlayerTargetFaction
 {

     override string GetText(SCR_NotificationData data)
     {
         int playerID, firstFactionIndex, secondFactionIndex;
         data.GetParams(playerID, firstFactionIndex, secondFactionIndex);

         string playerName, firstFactionName, secondFactionName;
         data.GetNotificationTextEntries(playerName, firstFactionName, secondFactionName);
         if (!GetPlayerName(playerID, playerName) || !GetFactionName(firstFactionIndex, firstFactionName) || !GetFactionName(secondFactionIndex, secondFactionName))
             return string.Empty;

         data.SetNotificationTextEntries(playerName, firstFactionName, secondFactionName);
         return super.GetText(data);
     }
 };