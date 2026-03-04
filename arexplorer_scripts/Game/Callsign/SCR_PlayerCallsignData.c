
 class SCR_PlayerCallsignData
 {
     protected int m_iCompanyIndex = -1;
     protected int m_iPlatoonIndex = -1;
     protected int m_iSquadIndex = -1;
     protected int m_iCharacterNumber = -1;
     protected ERoleCallsign m_eCharacterRole = ERoleCallsign.NONE;

     void SetPlayerCallsignIndexes(int company, int platoon, int squad, int character, ERoleCallsign role)
     {
         m_iCompanyIndex = company;
         m_iPlatoonIndex = platoon;
         m_iSquadIndex = squad;
         m_iCharacterNumber = character;
         m_eCharacterRole = role;
     }

     bool GetPlayerCallsignIndexes(out int company, out int platoon, out int squad, out int character, out ERoleCallsign role)
     {
         company = m_iCompanyIndex;
         platoon = m_iPlatoonIndex;
         squad = m_iSquadIndex;
         character = m_iCharacterNumber;
         role = m_eCharacterRole;

         return m_iCompanyIndex >= 0;
     }

     /*void SCR_PlayerCallsignData(Faction faction, SCR_CallsignManagerComponent callsignManager)
     {
         SetPlayerCallsign(faction, callsignManager);
     }

     /*void SetPlayerCallsign(Faction faction, SCR_CallsignManagerComponent callsignManager)
     {
         MakePlayerCallsignAvailible(callsignManager);

         m_Faction = faction;
         callsignManager.AssignCallGroupCallsign(faction, null, m_iCompanyIndex, m_iPlatoonIndex, m_iSquadIndex);
     }

     /*void RandomizeCallsign(Faction faction, SCR_CallsignManagerComponent callsignManager)
     {
         MakePlayerCallsignAvailible(callsignManager);

         m_Faction = faction;
         callsignManager.AssignRandomGroupCallsigns(faction, m_iCompanyIndex, m_iPlatoonIndex, m_iSquadIndex);
     }

     /*void MakePlayerCallsignAvailible(SCR_CallsignManagerComponent callsignManager)
     {
         if (m_Faction && m_iCompanyIndex >= 0)
             callsignManager.MakeGroupCallsignAvailible(m_Faction, m_iCompanyIndex, m_iPlatoonIndex, m_iSquadIndex);
     }*/


 };