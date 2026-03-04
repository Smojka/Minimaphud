
 [BaseContainerProps()]
 class SCR_FactionCallsignInfo
 {
     [Attribute(defvalue: "1", desc: "If true then callsigns will be assigned at random rather then trying to keep callsigns close")]
     protected bool m_bIsAssignedRandomly;

     [Attribute()]
     protected ref array<ref SCR_CallsignInfo> m_aCompanyNames;

     [Attribute(defvalue: "4", desc: "Index of overflow companies. Starting with the given index, these companies will only be assigned if all company indexes above are taken")]
     protected int m_iCompanyOverflowIndex;

     [Attribute()]
     protected ref array<ref SCR_CallsignInfo> m_aPlatoonNames;

     [Attribute()]
     protected ref array<ref SCR_CallsignInfo> m_aSquadNames;

     [Attribute(desc: "Used to show callsigns of groups. %1 = Company, %2 = Platoon, %3 = Squad")]
     protected LocalizedString m_sCallsignGroupFormat;

     [Attribute(desc: "Used when showing callsign of character that has no specific role. %1 = Company, %2 = Platoon, %3 = Squad, %4 = CharacterNumber")]
     protected LocalizedString m_sCallsignCharacterFormat;

     [Attribute(desc: "If character has a specific role this formating will be used instead. %1 = Company, %2 = Platoon, %3 = Squad, %4 = CharacterRole")]
     protected LocalizedString m_sCallsignCharacterWithRoleFormat;

     [Attribute(desc: "A character can have one role at the time and a group can only have one of each role. Note that roles are assigned in order, so make sure important roles (Such as leader) are on top of the list.")]
     protected ref array<ref SCR_BaseRoleCallsign> m_aCharacterRoleCallsigns;

     string GetCallsignFormat(bool includeCharacter, int characterRole = -1)
     {
         if (!includeCharacter)
             return m_sCallsignGroupFormat;
         else if (characterRole < 0)
             return m_sCallsignCharacterFormat;
         else
             return m_sCallsignCharacterWithRoleFormat;
     }

     bool GetIsAssignedRandomly()
     {
         return m_bIsAssignedRandomly;
     }

     void GetCompanyArray(notnull array<ref SCR_CallsignInfo> companyArray)
     {
         companyArray.Clear();

         foreach (SCR_CallsignInfo info: m_aCompanyNames)
             companyArray.Insert(info);
     }

     void GetPlatoonArray(notnull array<ref SCR_CallsignInfo> platoonArray)
     {
         platoonArray.Clear();

         foreach (SCR_CallsignInfo info: m_aPlatoonNames)
             platoonArray.Insert(info);
     }

     void GetSquadArray(notnull array<ref SCR_CallsignInfo> squadArray)
     {
         squadArray.Clear();

         foreach (SCR_CallsignInfo info: m_aSquadNames)
             squadArray.Insert(info);
     }

     string GetCompanyCallsignName(int index)
     {
         if (index < 0 || index >= m_aCompanyNames.Count())
             return index.ToString();

         return m_aCompanyNames[index].GetCallsign();
     }

     int GetCompanyOverflowIndex()
     {
         return m_iCompanyOverflowIndex;
     }

     string GetPlatoonCallsignName(int index)
     {
         if (index < 0 || index >= m_aPlatoonNames.Count())
             return index.ToString();

         return m_aPlatoonNames[index].GetCallsign();
     }

     string GetSquadCallsignName(int index)
     {
         if (index < 0 || index >= m_aSquadNames.Count())
             return index.ToString();

         return m_aSquadNames[index].GetCallsign();
     }

     string GetCharacterRoleCallsignName(int index)
     {
         if (!m_aCharacterRoleCallsigns.IsEmpty())
         {
             foreach (SCR_BaseRoleCallsign callsign: m_aCharacterRoleCallsigns)
             {
                 if (callsign.GetRoleIndex() == index)
                     return callsign.GetRoleName();
             }
         }

         Print(string.Format("Given Role index: '%1' does not exist in role data!", index.ToString()), LogLevel.ERROR);
         return index.ToString();
     }

     bool GetRandomCallsign(out int company, out int platoon, out int squad)
     {
         company = -1;
         platoon = -1;
         squad = -1;

         if (!m_aCompanyNames || m_aCompanyNames.Count() == 0 || !m_aPlatoonNames || m_aPlatoonNames.Count() == 0 || !m_aSquadNames || m_aSquadNames.Count() == 0)
             return false;

         //TODO: Make sure the same callsigns are never assigned twice
         Math.Randomize(-1);
         company = Math.RandomInt(0, m_aCompanyNames.Count());
         platoon = Math.RandomInt(0, m_aPlatoonNames.Count());
         squad = Math.RandomInt(0, m_aSquadNames.Count());

         return true;
     }

     bool GetCharacterRoleCallsign(IEntity character, int playerID, SCR_AIGroup group, inout int roleCallsignIndex, out bool isUnique)
     {
         foreach (SCR_BaseRoleCallsign roleCallsign: m_aCharacterRoleCallsigns)
         {
             if (roleCallsign.IsValidRole(character, playerID, group, roleCallsignIndex, isUnique))
                 return true;
         }

         return false;
     }
 };