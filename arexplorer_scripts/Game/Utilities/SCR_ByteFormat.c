 /*
 Useful functions to handle byte size formatting
 */

 class SCR_ByteFormat
 {
     const static float BYTE_UNIT_SIZE = 1024.0;

     const static string FORMAT_B ="#AR-DataSize_B";
     const static string FORMAT_KB ="#AR-DataSize_kB";
     const static string FORMAT_MB ="#AR-DataSize_MB";
     const static string FORMAT_GB = "#AR-DataSize_GB";

     //------------------------------------------------------------------------------------------------
  static string ReadableSizeFromBytes(out float value, int decimals = 1)
     {
         int safeCount = 4;
         int count = 0;

         float valCheck = value;

         while (value > BYTE_UNIT_SIZE && count < safeCount)
         {
             value /= BYTE_UNIT_SIZE;
             count++;
         }

         // Cut last 1000
         if (value * 0.001 > 1)
         {
             value *= 0.001;
             count++;
         }

         valCheck = value;

         // First decimal place
         if (decimals > 0)
         {
             float dec = Math.Pow(10, decimals);
             float valueMult10 = Math.Ceil(value * dec);

             if (valueMult10 <= dec)
                 value = valueMult10 / dec;
             else
                 value = valueMult10 / dec;
         }

         // Add proper metrics
         switch (count)
         {
             // byte
             case 0: return FORMAT_B; break;

             // kilo
             case 1: return FORMAT_KB; break;

             // mega
             case 2: return FORMAT_MB; break;

             // giga
             case 3: return FORMAT_GB; break;
         }

         return "";
     }


     //------------------------------------------------------------------------------------------------
  static string GetReadableSize(float value, int decimals = 1)
     {
         string unit = SCR_ByteFormat.ReadableSizeFromBytes(value, decimals); // format %1 unit
         return WidgetManager.Translate(unit, value);
     }

     //------------------------------------------------------------------------------------------------
  static string GetReadableSizeMb(float value)
     {
         float mb = 1024.0*1024.0;
         float sizeMb = value / mb;

         if (sizeMb < 1.0)
         {
             int sizeMbMult10 = Math.Ceil(sizeMb * 10.0);

             if (sizeMbMult10 <= 10)
                 sizeMb = sizeMbMult10 / 10;

             return WidgetManager.Translate(FORMAT_MB, sizeMb);
         }
         else
             return  WidgetManager.Translate(FORMAT_MB, Math.Round(sizeMb));
     }

     //------------------------------------------------------------------------------------------------
  static string ContentDownloadFormat(float value)
     {
         string unit = SCR_ByteFormat.ReadableSizeFromBytes(value, 3);
         string size = value.ToString();

         int dec = 3;

         if (unit != SCR_ByteFormat.FORMAT_GB)
             dec = 1;

         size = value.ToString(-1, dec);
         return  WidgetManager.Translate(unit, size);
     }
 };