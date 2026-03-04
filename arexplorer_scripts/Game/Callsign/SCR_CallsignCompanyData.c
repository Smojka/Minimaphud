
 class SCR_CallsignCompanyData
 {
     protected ref map<int, ref SCR_CallsignPlatoonData> m_mPlatoonCallsigns = new map<int, ref SCR_CallsignPlatoonData>;

     //---------------------------------------- On Init ----------------------------------------\\

  void Init(SCR_FactionCallsignInfo factionCallsignInfo)
     {
         array<ref SCR_CallsignInfo> platoonArray = new array<ref SCR_CallsignInfo>;
         factionCallsignInfo.GetPlatoonArray(platoonArray);
         int count = platoonArray.Count();

         for(int i = 0; i < count; i++)
         {
             SCR_CallsignPlatoonData platoonData = SCR_CallsignPlatoonData();
             platoonData.Init(factionCallsignInfo);
             m_mPlatoonCallsigns.Insert(i, platoonData);
         }
     }

     //---------------------------------------- Get Random Callsign ----------------------------------------\\

  void GetRandomCallsign(out int platoon, out int squad)
     {
         Math.Randomize(-1);
         int random = Math.RandomInt(0, m_mPlatoonCallsigns.Count());
         platoon = m_mPlatoonCallsigns.GetKey(random);
         SCR_CallsignPlatoonData randomPlatoon = m_mPlatoonCallsigns.GetElement(random);

         squad = randomPlatoon.GetRandomSquad();
     }

     //---------------------------------------- Get first availible Callsign ----------------------------------------\\

  void GetFirstAvailibleCallsign(out int firstAvailiblePlatoon, out int firstAvailibleSquad)
     {
         firstAvailiblePlatoon = int.MAX;
         SCR_CallsignPlatoonData firstAvailiblePlatoonData;

         foreach (int index, SCR_CallsignPlatoonData platoon: m_mPlatoonCallsigns)
         {
             if (index < firstAvailiblePlatoon)
             {
                 firstAvailiblePlatoon = index;
                 firstAvailiblePlatoonData = platoon;
             }
         }

         firstAvailibleSquad = firstAvailiblePlatoonData.GetFirstAvailibleSquad();
     }

     //---------------------------------------- Add availible Callsign ----------------------------------------\\

  void AddCallsign(int platoonIndex, int squadIndex)
     {
         SCR_CallsignPlatoonData platoonData;

         if (m_mPlatoonCallsigns.Find(platoonIndex, platoonData))
         {
             platoonData.AddSquad(squadIndex);
         }
         else
         {
             platoonData = new SCR_CallsignPlatoonData();
             platoonData.AddSquad(squadIndex);
             m_mPlatoonCallsigns.Insert(platoonIndex, platoonData);
         }
     }

     //---------------------------------------- Remove availible Callsign ----------------------------------------\\

  bool RemoveCallsign(int plattonIndex, int squadIndex)
     {
         SCR_CallsignPlatoonData platoonData;

         if (m_mPlatoonCallsigns.Find(plattonIndex, platoonData))
         {
             //Check if all squads are used then remove platoon
             if (platoonData.RemoveSquad(squadIndex))
             {
                 m_mPlatoonCallsigns.Remove(plattonIndex);
                 return m_mPlatoonCallsigns.IsEmpty();
             }
         }

         return false;
     }
 };