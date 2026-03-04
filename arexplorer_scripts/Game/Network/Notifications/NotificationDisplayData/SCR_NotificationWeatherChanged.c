
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationWeatherChanged : SCR_NotificationPlayer
 {
     override string GetText(SCR_NotificationData data)
     {
         int playerID, weatherIndex;
         data.GetParams(playerID, weatherIndex);

         string playerName;
         data.GetNotificationTextEntries(playerName);
         if (!GetPlayerName(playerID, playerName))
             return string.Empty;

         ChimeraWorld world = GetGame().GetWorld();
         TimeAndWeatherManagerEntity weatherManager = world.GetTimeAndWeatherManager();
         if (!weatherManager)
             return string.Empty;

         array<ref WeatherState> weatherStates = new array<ref WeatherState>;
         weatherManager.GetWeatherStatesList(weatherStates);
         if (weatherIndex >= weatherStates.Count() )
             return string.Empty;

         data.SetNotificationTextEntries(playerName, weatherStates[weatherIndex].GetLocalizedName());

         return super.GetText(data);
     }
 };