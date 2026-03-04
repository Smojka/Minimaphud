 class SCR_LatestSaveStruct: JsonApiStruct
 {
     ref array<ref SCR_LatestSaveEntryStruct> m_aEntries = {};

     //----------------------------------------------------------------------------------------
  void SetFileName(string missionFileName, string saveFileName)
     {
         SCR_LatestSaveEntryStruct entry;
         foreach (SCR_LatestSaveEntryStruct entryCandidate: m_aEntries)
         {
             if (entryCandidate.wFn == missionFileName)
             {
                 entry = entryCandidate;
                 break;
             }
         }
         if (!entry)
         {
             entry = new SCR_LatestSaveEntryStruct();
             entry.wFn = missionFileName;
             m_aEntries.Insert(entry);
         }
         entry.sFn = saveFileName;
     }

     //----------------------------------------------------------------------------------------
  void RemoveFileName(string missionFileName)
     {
         for (int i = m_aEntries.Count() - 1; i >= 0; i--)
         {
             if (m_aEntries[i].wFn == missionFileName)
             {
                 m_aEntries.Remove(i);
                 break;
             }
         }
     }

     //----------------------------------------------------------------------------------------
  bool FindFileName(string missionFileName, out string outSaveFileName)
     {
         for (int i = m_aEntries.Count() - 1; i >= 0; i--)
         {
             if (m_aEntries[i].wFn == missionFileName)
             {
                 outSaveFileName = m_aEntries[i].sFn;
                 return true;
             }
         }
         return false;
     }

     //----------------------------------------------------------------------------------------
  void Log()
     {
         int count = m_aEntries.Count();
         PrintFormat("Latest saves: %1", count);
         for (int i = count - 1; i >= 0; i--)
         {
             PrintFormat("  %1: %2", m_aEntries[i].wFn, m_aEntries[i].sFn);
         }
     }

     //----------------------------------------------------------------------------------------
     void SCR_LatestSaveStruct()
     {
         RegV("m_aEntries");
     }
 }
 class SCR_LatestSaveEntryStruct: JsonApiStruct
 {
     string wFn; //--- World file name
     string sFn; //--- Save file name

     void SCR_LatestSaveEntryStruct()
     {
         RegAll();
     }
 }