 class SCR_DateTimeHelper
 {
     //------------------------------------------------------------------------------------------------
  static string GetDateTimeLocal()
     {
         int year, month, day, hour, minute, second;
         System.GetYearMonthDay(year, month, day);
         System.GetHourMinuteSecond(hour, minute, second);
         return SCR_FormatHelper.FormatDateTime(year, month, day, hour, minute, second);
     }

     //------------------------------------------------------------------------------------------------
  static string GetDateTimeUTC()
     {
         int year, month, day, hour, minute, second;
         System.GetYearMonthDayUTC(year, month, day);
         System.GetHourMinuteSecondUTC(hour, minute, second);
         return SCR_FormatHelper.FormatDateTime(year, month, day, hour, minute, second);
     }

     //------------------------------------------------------------------------------------------------
  static string GetDateString(int day, int month, int year, bool verbose = true)
     {
         string date;

         if (verbose)
             date = WidgetManager.Translate("#AR-Date_Format_MonthFull", day, SCR_DateTimeHelper.GetMonthString(month), year);
         else
             date = WidgetManager.Translate("#AR-Date_Format", day, month, year);

         return date;
     }


     //------------------------------------------------------------------------------------------------
  static string GetMonthString(int month, bool standalone = false)
     {
         if (month < 1 || month > 12)
             return string.Empty;

         string months[12];

         if (standalone)
         {
             months = {
                 "#AR-Date_January_Standalone",
                 "#AR-Date_February_Standalone",
                 "#AR-Date_March_Standalone",
                 "#AR-Date_April_Standalone",
                 "#AR-Date_May_Standalone",
                 "#AR-Date_June_Standalone",
                 "#AR-Date_July_Standalone",
                 "#AR-Date_August_Standalone",
                 "#AR-Date_September_Standalone",
                 "#AR-Date_October_Standalone",
                 "#AR-Date_November_Standalone",
                 "#AR-Date_December_Standalone",
             };
         }
         else
         {
             months = {
                 "#AR-Date_January",
                 "#AR-Date_February",
                 "#AR-Date_March",
                 "#AR-Date_April",
                 "#AR-Date_May",
                 "#AR-Date_June",
                 "#AR-Date_July",
                 "#AR-Date_August",
                 "#AR-Date_September",
                 "#AR-Date_October",
                 "#AR-Date_November",
                 "#AR-Date_December",
             };
         }

         return months[month -1];
     }

     //------------------------------------------------------------------------------------------------
  static string GetAbbreviatedMonthString(int month)
     {
         if (month < 1 || month > 12)
             return string.Empty;

         // if it gets called often, cache it as a static array
         array<string> months = {
             "#AR-Date_January_StandaloneShort",
             "#AR-Date_February_StandaloneShort",
             "#AR-Date_March_StandaloneShort",
             "#AR-Date_April_StandaloneShort",
             "#AR-Date_May_StandaloneShort",
             "#AR-Date_June_StandaloneShort",
             "#AR-Date_July_StandaloneShort",
             "#AR-Date_August_StandaloneShort",
             "#AR-Date_September_StandaloneShort",
             "#AR-Date_October_StandaloneShort",
             "#AR-Date_November_StandaloneShort",
             "#AR-Date_December_StandaloneShort",
         };

         return months[month -1];
     }

     //------------------------------------------------------------------------------------------------
  static int GetTimeDifference(int hour0, int minute0, int second0, int hour1, int minute1, int second1, out int hour = 0, out int minute = 0, out int second = 0)
     {
         int time0 = GetSecondsFromHourMinuteSecond(hour0, minute0, second0);
         int time1 = GetSecondsFromHourMinuteSecond(hour1, minute1, second1);

         int result = time0 - time1;
         if (result < 0)
             result *= -1;

         GetHourMinuteSecondFromSeconds(result, hour, minute, second);

         return result;
     }

     //------------------------------------------------------------------------------------------------
  static string GetTimeDifferenceFormatted(int hour0, int minute0, int second0, int hour1, int minute1, int second1)
     {
         int hour, minute, second;
         GetTimeDifference(hour0, minute0, second0, hour1, minute1, second1, hour, minute, second);
         return SCR_FormatHelper.FormatTime(hour, minute, second);
     }

     //------------------------------------------------------------------------------------------------
  static string GetTimeLocal()
     {
         int hour, minute, second;
         System.GetHourMinuteSecond(hour, minute, second);
         return SCR_FormatHelper.FormatTime(hour, minute, second);
     }

     //------------------------------------------------------------------------------------------------
  static string GetTimeUTC()
     {
         int hour, minute, second;
         System.GetHourMinuteSecondUTC(hour, minute, second);
         return SCR_FormatHelper.FormatTime(hour, minute, second);
     }

     //------------------------------------------------------------------------------------------------
  static void GetDayHourMinuteSecondFromSeconds(int totalSeconds, out int outDays, out int outHours, out int outMinutes, out int outSeconds)
     {
         if (totalSeconds < 0)
             totalSeconds *= -1;

         outDays = totalSeconds / 86400;
         outHours = (totalSeconds % 86400) / 3600;
         outMinutes = (totalSeconds % 3600) / 60;
         outSeconds = totalSeconds % 60;
     }

     //------------------------------------------------------------------------------------------------
  static void GetHourMinuteSecondFromSeconds(int totalSeconds, out int outHours, out int outMinutes, out int outSeconds)
     {
         if (totalSeconds < 0)
             totalSeconds *= -1;

         outHours = totalSeconds / 3600;
         outMinutes = (totalSeconds % 3600) / 60;
         outSeconds = totalSeconds % 60;
     }

     //------------------------------------------------------------------------------------------------
  static int GetSecondsFromHourMinuteSecond(int hour = 0, int minute = 0, int second = 0)
     {
         return hour * 3600 + minute * 60 + second;
     }

     //------------------------------------------------------------------------------------------------
  static int ConvertDateIntoMinutes(int year = 0, int month = 0, int day = 0, int hour = 0, int minutes = 0)
     {
         return (year * 525600) + (month * 43800) + (day * 1440) + (hour * 60) + minutes;
     }

     //------------------------------------------------------------------------------------------------
  static void ConvertMinutesIntoDate(int totalDateMinutes, out int year, out int month, out int day, out int hour, out int minutes)
     {
         year = totalDateMinutes / 525600;
         totalDateMinutes -= year * 525600;

         month = totalDateMinutes / 43800;
         totalDateMinutes -= month * 43800;

         day = totalDateMinutes / 1440;
         totalDateMinutes -= day * 1440;

         hour = totalDateMinutes / 60;
         totalDateMinutes -= hour * 60;

         minutes = totalDateMinutes;
     }
 };