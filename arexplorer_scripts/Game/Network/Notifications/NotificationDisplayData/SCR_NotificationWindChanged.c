
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationWindChanged : SCR_NotificationPlayer
 {
     override string GetText(SCR_NotificationData data)
     {
         int playerID, windSpeed, windDirectionIndex;
         string windDirection = "UNKNOWN";
         data.GetParams(playerID, windSpeed, windDirectionIndex);

         string playerName;
         data.GetNotificationTextEntries(playerName);
         if (!GetPlayerName(playerID, playerName))
             return string.Empty;

         ChimeraWorld world = GetGame().GetWorld();
         TimeAndWeatherManagerEntity weatherManager = world.GetTimeAndWeatherManager();
         SCR_WindDirectionInfo windDirectionInfo;
         if (weatherManager && weatherManager.GetWindDirectionInfoFromIndex(windDirectionIndex, windDirectionInfo))
             windDirection = windDirectionInfo.GetUIInfo().GetName();

         float windSpeedFloat = windSpeed / 1000;

         data.SetNotificationTextEntries(playerName, windSpeedFloat.ToString(), windDirection);

         return super.GetText(data);
     }
 };