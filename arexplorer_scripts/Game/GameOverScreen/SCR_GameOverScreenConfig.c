 [BaseContainerProps(configRoot: true)]
 class SCR_GameOverScreenConfig
 {
     [Attribute()]
     protected ref array<ref SCR_BaseGameOverScreenInfo> m_aGameOverScreens;

     protected ref map<EGameOverTypes, SCR_BaseGameOverScreenInfo> m_mGameOverScreensMap = new map<EGameOverTypes, SCR_BaseGameOverScreenInfo>;


     bool GetGameOverScreenInfo(EGameOverTypes infoId, out SCR_BaseGameOverScreenInfo gameOverScreenInfo)
     {
         if (!m_mGameOverScreensMap.Find(infoId, gameOverScreenInfo))
             return false;
         else
             return true;
     }


     int GetGameOverInfoArray(notnull out array <SCR_BaseGameOverScreenInfo> gameOverScreens)
     {
         foreach (SCR_BaseGameOverScreenInfo screen: m_aGameOverScreens)
             gameOverScreens.Insert(screen);

         return gameOverScreens.Count();
     }

     SCR_BaseGameOverScreenInfo GetGameOverInfo(EGameOverTypes infoId)
     {
         SCR_BaseGameOverScreenInfo info = null;

         m_mGameOverScreensMap.Find(infoId, info);

         return info;
     }

     //Init
     protected void SCR_GameOverScreenConfig()
     {
         EGameOverTypes id;
         int count = m_aGameOverScreens.Count();

         for(int i = 0; i <count; i++)
         {
             id = m_aGameOverScreens[i].GetInfoId();

             //If empty
             if (id == EGameOverTypes.UNKNOWN)
             {
                 Print(string.Format("'GameEndState.conf' contains a state with an UNKNOWN state id at index '%1'", i.ToString()), LogLevel.ERROR);
                 continue;
             }
             //If duplicate
             else if (m_mGameOverScreensMap.Contains(id))
             {
                 Print(string.Format("'GameEndState.conf' contains duplicate end state id: '%1' at index '%2'", id, i.ToString()), LogLevel.ERROR);
                 continue;
             }

             m_mGameOverScreensMap.Insert(id, m_aGameOverScreens[i]);

         }
     }
 };