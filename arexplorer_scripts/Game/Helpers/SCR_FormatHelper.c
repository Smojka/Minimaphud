 class SCR_FormatHelper
 {
     //------------------------------------------------------------------------------------------------
  static string FloatToStringNoZeroDecimalEndings(float value, int lenDec)
     {
         return value.ToString(lenDec: lenDec).ToFloat().ToString();
     }

     //------------------------------------------------------------------------------------------------
  static string FormatDateTime(int year, int month, int day, int hour, int minute, int second)
     {
         return string.Format("%1 %2", FormatDate(year, month, day), FormatTime(hour, minute, second));
     }

     //------------------------------------------------------------------------------------------------
  static string FormatDate(int year, int month, int day)
     {
         if (year < 100)
             year += 2000;
         return string.Format("%1-%2-%3", year.ToString(4), month.ToString(2), day.ToString(2));
     }

     //------------------------------------------------------------------------------------------------
  static string FormatTime(int totalSeconds)
     {
         int hour, minute, second;
         SCR_DateTimeHelper.GetHourMinuteSecondFromSeconds(totalSeconds, hour, minute, second);
         return FormatTime(hour, minute, second);
     }

     //------------------------------------------------------------------------------------------------
  static string FormatTime(int hour, int minute, int second)
     {
         return string.Format("%1:%2:%3", hour.ToString(2), minute.ToString(2), second.ToString(2));
     }

     //------------------------------------------------------------------------------------------------
  static string GetTimeFormatting(int days, int hours, int minutes, int seconds, ETimeFormatParam hideEmpty = 0, ETimeFormatParam hideLeadingZeroes = 0)
     {
         //Todo: Display days properly rather then dd:hh:mm:ss (Though right now it is extreamly edge case if days should be shown)
         string returnString = "";

         //To do: Decide the formatting of days. Currently it is dd:hh:mm:ss and by default days are hidden if it is 0 as it would rarely be a day or more
         //Only show days
         //if (days > -1 && ((hideEmpty & ETimeFormatParam.DAYS) && days > 0) || !(hideEmpty & ETimeFormatParam.DAYS))
         if (days > 0 && (hideEmpty & ETimeFormatParam.DAYS) || !(hideEmpty & ETimeFormatParam.DAYS))
             returnString += days.ToString();
         else
             days = -1;

         //hours (Check if should display hours and check if should display leading zero)
         if (hours > 0 && (hideEmpty & ETimeFormatParam.HOURS) || !(hideEmpty & ETimeFormatParam.HOURS))
             returnString += ReturnTimeTypeString(days, hours, ((hideLeadingZeroes & ETimeFormatParam.HOURS) == 0) +1);
         else
             hours = -1;

         //Minutes (Check if should display Minutes and check if should display leading zero)
         if (minutes > 0 && (hideEmpty & ETimeFormatParam.MINUTES) || !(hideEmpty & ETimeFormatParam.MINUTES))
             returnString += ReturnTimeTypeString(hours, minutes, ((hideLeadingZeroes & ETimeFormatParam.MINUTES) == 0) +1);
         else
             minutes = -1;

         //Seconds (Check if should display Seconds and check if should display leading zero)
         if (seconds > 0 && (hideEmpty & ETimeFormatParam.SECONDS) || !(hideEmpty & ETimeFormatParam.SECONDS))
             returnString += ReturnTimeTypeString(minutes, seconds, ((hideLeadingZeroes & ETimeFormatParam.SECONDS) == 0) +1);

         return returnString;
     }

     //------------------------------------------------------------------------------------------------
  protected static string ReturnTimeTypeString(int prevTimeTypeAmount, int currentTimeTypeAmount, int numberLength)
     {
         if (prevTimeTypeAmount > -1)
             return ":" + currentTimeTypeAmount.ToString(numberLength);
         else
             return currentTimeTypeAmount.ToString(numberLength);
     }

     //------------------------------------------------------------------------------------------------
  static string GetTimeFormatting(int totalSeconds, ETimeFormatParam hideEmpty = 0, ETimeFormatParam hideLeadingZeroes = 0)
     {
         int days, hours, minutes, seconds;
         SCR_DateTimeHelper.GetDayHourMinuteSecondFromSeconds(totalSeconds, days, hours, minutes, seconds);
         return GetTimeFormatting(days, hours, minutes, seconds, hideEmpty, hideLeadingZeroes);
     }

     //------------------------------------------------------------------------------------------------
  static string GetTimeFormattingHideSeconds(int totalSeconds, ETimeFormatParam hideEmpty = 0, ETimeFormatParam hideLeadingZeroes = 0)
     {
         int days, hours, minutes, seconds;
         SCR_DateTimeHelper.GetDayHourMinuteSecondFromSeconds(totalSeconds, days, hours, minutes, seconds);
         return GetTimeFormatting(days, hours, minutes, -1, ETimeFormatParam.SECONDS | hideEmpty, hideLeadingZeroes);
     }

     //------------------------------------------------------------------------------------------------
  static string GetTimeFormattingHoursMinutes(int hours, int minutes, ETimeFormatParam hideEmpty = 0, ETimeFormatParam hideLeadingZeroes = 0)
     {
         return GetTimeFormatting(-1, hours, minutes, -1, (ETimeFormatParam.DAYS | ETimeFormatParam.SECONDS | hideEmpty), hideLeadingZeroes);
     }

     //------------------------------------------------------------------------------------------------
  static string GetTimeFormattingMinutesSeconds(int minutes, int seconds, ETimeFormatParam hideEmpty = 0, ETimeFormatParam hideLeadingZeroes = 0)
     {
         return GetTimeFormatting(-1, -1, minutes, seconds, (ETimeFormatParam.DAYS | ETimeFormatParam.HOURS | hideEmpty), hideLeadingZeroes);
     }

     //------------------------------------------------------------------------------------------------
  protected static string GetTimeSinceEventString(int amount, string oneUnit, string manyUnits)
     {
         if (amount == 1)
             return WidgetManager.Translate(oneUnit, amount);
         else
             return WidgetManager.Translate(manyUnits, amount);
     }

     //------------------------------------------------------------------------------------------------
  static string GetTimeSinceEventImprecise(int timeDiffSeconds)
     {
         if (timeDiffSeconds < 0)
             return string.Empty;

         int days, hours, minutes, remainingSeconds;
         SCR_DateTimeHelper.GetDayHourMinuteSecondFromSeconds(timeDiffSeconds, days, hours, minutes, remainingSeconds);

         if (days > 0)
             return GetTimeSinceEventString(days, "#AR-Date_DayAgo_LC", "#AR-Date_DaysAgo_LC");          // 23 days ago
         else if (hours > 0)
             return GetTimeSinceEventString(hours, "#AR-Time_HourAgo_LC", "#AR-Time_HoursAgo_LC");       // 23 hours ago
         else
             return GetTimeSinceEventString(minutes, "#AR-Time_MinuteAgo_LC", "#AR-Time_MinutesAgo_LC"); // 23 minutes ago
     }

     //------------------------------------------------------------------------------------------------
  static string FormatFrequencies(notnull set<int> frequencies, set<int> highlightFrequencies = null)
     {
         if (frequencies.IsEmpty())
             return string.Empty;

         string text;
         float accurateFrequency;
         foreach (int i, int frequency : frequencies)
         {
             if (i > 0)
                 text += ", ";
             accurateFrequency = frequency;
             if (highlightFrequencies && highlightFrequencies.Contains(frequency))
                 text += string.Format("<color rgba='226,168,79,255'>%1</color>", accurateFrequency / 1000); //--- ToDo: Don't hardcode color
             else
                 text += (accurateFrequency / 1000).ToString();
         }
         return text + " #AR-VON_FrequencyUnits_MHz";
     }
 };