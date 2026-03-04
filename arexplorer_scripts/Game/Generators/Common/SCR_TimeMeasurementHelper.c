 class SCR_TimeMeasurementHelper
 {
     protected ref map<string, ref SCR_TimeMeasurementHelper_Info> m_mData;

     //------------------------------------------------------------------------------------------------
  void BeginMeasure(string name)
     {
         SCR_TimeMeasurementHelper_Info info = m_mData.Get(name);
         if (!info)
         {
             info = new SCR_TimeMeasurementHelper_Info();
             m_mData.Insert(name, info);
         }

         info.m_fStart = System.GetTickCount();
     }

     //------------------------------------------------------------------------------------------------
  void EndMeasure(string name)
     {
         SCR_TimeMeasurementHelper_Info info = m_mData.Get(name);
         if (!info)
         {
             Print("No measurement named \"" + name + "\" was found", LogLevel.WARNING);
             return;
         }

         info.m_fTotal += System.GetTickCount(info.m_fStart);
     }

     //------------------------------------------------------------------------------------------------
  float GetMeasure(string name)
     {
         SCR_TimeMeasurementHelper_Info info = m_mData.Get(name);
         if (!info)
         {
             Print("No measurement named \"" + name + "\" was found", LogLevel.WARNING);
             return -1;
         }

         return info.m_fTotal;
     }

     //------------------------------------------------------------------------------------------------
  void PrintAllMeasures()
     {
         foreach (string name, SCR_TimeMeasurementHelper_Info info : m_mData)
         {
             Print("Measure \"" + name + "\": " + info.m_fTotal + " ms", LogLevel.NORMAL);
         }
     }

     //------------------------------------------------------------------------------------------------
  void Reset()
     {
         m_mData.Clear();
     }

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_TimeMeasurementHelper()
     {
         m_mData = new map<string, ref SCR_TimeMeasurementHelper_Info>();
     }
 }

 // databag
 class SCR_TimeMeasurementHelper_Info
 {
     float m_fStart;
     float m_fTotal;
 }