 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseTimeAndWeatherManagerEntityClass: BaseWeatherManagerEntityClass
 {
 }

 class BaseTimeAndWeatherManagerEntity: BaseWeatherManagerEntity
 {
     proto external ref WeatherState GetCurrentWeatherState();

     //Returns true if the hour is inside the nighttime.
     proto external bool IsNightHour(float hour24);
     //Returns true if the hour is inside the daytime.
     proto external bool IsDayHour(float hour24);
     // Defines a callback that should be invoked at the specified time (format should be "HHMMSS" and TimeAndWeatherManager periodicity should be set accordingly)
     proto void SetTimeEvent(string time, Managed inst, func callback, bool delayed = true, bool singleUse = false);
     proto external float GetMoonPhase(float timeOfTheDay24);
     proto float GetMoonPhaseForDate(int year, int month, int day, float timeOfTheDay24, float timezone, float dstOffset);
     proto external bool SetDate(int year, int month, int day, bool immediateChange = false);
     proto external void UpdateWeather(float timeSlice);
     proto external bool SetTimeOfTheDay(float hours24, bool immediateChange = false);
     proto external bool CheckValidDate(int year, int month, int day);
     proto void GetCurrentSunMoonDirAndPhase(out vector outSunDir, out vector outMoonDir, out float outMoonPhase01);
     proto external float GetTimeOfTheDay();
     proto external float GetEngineTime();
     proto external int GetYear();
     proto external int GetMonth();
     proto external int GetDay();
     proto external int GetWeekDay();
     proto external int GetWeekDayForDate(int year, int month, int day);
     proto external float GetTimeZoneOffset();
     proto external void SetTimeZoneOffset(float utcTimeZone);
     proto external float GetDSTOffset();
     proto external bool SetDSTOffset(float dstOffsetHours);
     proto external bool IsDSTEnabled();
     proto external bool SetDSTEnabled(bool enabled);
     proto bool GetSunriseHour(out float hour24);
     proto bool GetSunsetHour(out float hour24);
     proto bool GetSunriseHourForDate(int year, int month, int day, float latitude, float longitude, float timezone, float dstOffset, out float hour24);
     proto bool GetSunsetHourForDate(int year, int month, int day, float latitude, float longitude, float timezone, float dstOffset, out float hour24);
     proto external bool TryGetCompleteLocalWeather(LocalWeatherSituation lws, float swayFrequency, vector location);
     proto external bool SetDayDuration(float realtimeSeconds);
     proto external float GetDayDuration();
     proto external bool SetIsDayAutoAdvanced(bool autoAdvanced);
     proto external bool GetIsDayAutoAdvanced();
     proto external bool SetCurrentLongitude(float longitude);
     proto external float GetCurrentLongitude();
     proto external bool SetCurrentLatitude(float latitude);
     proto external float GetCurrentLatitude();
     proto external bool SetHoursMinutesSeconds(int hours, int minutes, int seconds, bool immediateChange = false);
     proto void GetDate(out int year, out int month, out int day);
     proto void GetHoursMinutesSeconds(out int hours, out int minutes, out int seconds);
     /*
     Converts in-game time of the day to hours, minutes and seconds.
     \param hours24 In-game time as a fraction of day <0.0, 24.0>
     */
     static proto void TimeToHoursMinutesSeconds(float hours24, out int hours, out int minutes, out int seconds);
     proto external void GetWeatherStatesList(out notnull array<ref WeatherState> outStates);
     proto ref WeatherStateTransitionManager GetTransitionManager();
     /*
     Converts hours, minutes and seconds to in-game time of the day.
     \return Returns time of the day as a fraction of day <0.0, 24.0>
     */
     static proto float HoursMinutesSecondsToTime(int hours, int minutes, int seconds);
     proto external bool SetWindSpeedOverride(bool doOverride, float windSpeed = 0);
     proto external float GetWindSpeed();
     proto external bool SetWindDirectionOverride(bool doOverride, float windDirection = 0);
     proto external float GetWindDirection();
     proto external bool IsWindSpeedOverridden();
     proto external bool IsWindDirectionOverridden();
     proto external bool SetRainIntensityOverride(bool doOverride, float rainIntensity = 0);
     proto external float GetRainIntensity();
     proto external bool IsRainIntensityOverridden();
     /*
     Sets current wetness (in <0,1>)
     Can only be set by the authority (server, singleplayer...)
     \return Returns true when command is authorized, false otherwise.
     */
     proto external bool SetCurrentWetness(float wetness);
     /*
     Gets current wetness (in <0,1>)
     */
     proto external float GetCurrentWetness();
     /*
     Sets current water accumulation(in <0,1>)
     Can only be set by the authority (server, singleplayer...)
     \param waterAccumulationCracks water accumulation value in cracks
     \param waterAccumulationPuddles water accumulation value in puddles
     \return Returns true when command is authorized, false otherwise.
     */
     proto external bool SetCurrentWaterAccumulation(float waterAccumulationCracks, float waterAccumulationPuddles);
     /*
     Gets current water accumulation in cracks (in <0,1>)
     */
     proto external float GetCurrentWaterAccumulationCracks();
     /*
     Gets current water accumulation in puddles (in <0,1>)
     */
     proto external float GetCurrentWaterAccumulationPuddles();
     proto external void AddLightning(WeatherLightning lightning);
 }
