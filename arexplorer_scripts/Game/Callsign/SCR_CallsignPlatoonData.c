
 class SCR_CallsignPlatoonData
 {
     protected ref array<int> m_mSquadCallsigns = new array<int>;

     //---------------------------------------- On Init ----------------------------------------\\

  void Init(SCR_FactionCallsignInfo factionCallsignInfo)
     {
         array<ref SCR_CallsignInfo> squadArray = new array<ref SCR_CallsignInfo>;
         factionCallsignInfo.GetSquadArray(squadArray);
         int count = squadArray.Count();

         for(int i = 0; i < count; i++)
         {
             m_mSquadCallsigns.Insert(i);
         }
     }

     //---------------------------------------- Get random squad Callsign ----------------------------------------\\

  int GetRandomSquad()
     {
         return m_mSquadCallsigns[Math.RandomInt(0, m_mSquadCallsigns.Count())];
     }

     int GetFirstAvailibleSquad()
     {
         int firstAvailible = int.MAX;

         foreach (int squad: m_mSquadCallsigns)
         {
             if (squad < firstAvailible)
                 firstAvailible = squad;
         }

         return firstAvailible;
     }

     //---------------------------------------- Add availible squad Callsign ----------------------------------------\\

  void AddSquad(int squadIndex)
     {
         if (!m_mSquadCallsigns.Contains(squadIndex))
             m_mSquadCallsigns.Insert(squadIndex);
     }

     //---------------------------------------- Remove availible squad Callsign ----------------------------------------\\

  bool RemoveSquad(int squadIndex)
     {
         int count = m_mSquadCallsigns.Count();

         for(int i = 0; i < count; i++)
         {
             if (m_mSquadCallsigns[i] == squadIndex)
             {
                 m_mSquadCallsigns.Remove(i);
                 break;
             }
         }

         return m_mSquadCallsigns.IsEmpty();
     }
 };