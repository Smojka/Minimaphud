
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationPlayerBanned : SCR_NotificationPlayer
 {
     override string GetText(SCR_NotificationData data)
     {
         int playerID, duration;
         data.GetParams(playerID, duration);

         int days, hours, minutes, seconds;
         SCR_DateTimeHelper.GetDayHourMinuteSecondFromSeconds(duration, days, hours, minutes, seconds);

         string playerName;
         data.GetNotificationTextEntries(playerName);
         if (!GetPlayerName(playerID, playerName))
             return string.Empty;

         data.SetNotificationTextEntries(playerName, days.ToString(),  hours.ToString(),  minutes.ToString(),  seconds.ToString());
         return super.GetText(data);
     }

 };