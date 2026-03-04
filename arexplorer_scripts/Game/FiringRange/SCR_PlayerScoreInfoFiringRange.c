 //------------------------------------------------------------------------------------------------
 class SCR_PlayerScoreInfoFiringRange
 {
     int m_iID = -1;
     int m_iScore = 0;
     int m_iScoreMax = 0;

     //################################################################################################
  //------------------------------------------------------------------------------------------------
     static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet)
     {
         snapshot.Serialize(packet, 12);
     }

     //------------------------------------------------------------------------------------------------
     static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot)
     {
         return snapshot.Serialize(packet, 12);
     }

     //------------------------------------------------------------------------------------------------
     static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx ctx)
     {
         return lhs.CompareSnapshots(rhs, 12);
     }

     //------------------------------------------------------------------------------------------------
     static bool PropCompare(SCR_PlayerScoreInfoFiringRange prop, SSnapSerializerBase snapshot, ScriptCtx ctx)
     {
         return snapshot.Compare(prop.m_iID, 4)
             && snapshot.Compare(prop.m_iScore, 4)
             && snapshot.Compare(prop.m_iScoreMax, 4);
     }

     //------------------------------------------------------------------------------------------------
     static bool Extract(SCR_PlayerScoreInfoFiringRange prop, ScriptCtx ctx, SSnapSerializerBase snapshot)
     {
         snapshot.SerializeBytes(prop.m_iID, 4);
         snapshot.SerializeBytes(prop.m_iScore, 4);
         snapshot.SerializeBytes(prop.m_iScoreMax, 4);
         return true;
     }

     //------------------------------------------------------------------------------------------------
     static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, SCR_PlayerScoreInfoFiringRange prop)
     {
         snapshot.SerializeBytes(prop.m_iID, 4);
         snapshot.SerializeBytes(prop.m_iScore, 4);
         snapshot.SerializeBytes(prop.m_iScoreMax, 4);

         return true;
     }

     //################################################################################################
     //------------------------------------------------------------------------------------------------
  string GetName()
     {
         return GetGame().GetPlayerManager().GetPlayerName(m_iID);
     }

     //------------------------------------------------------------------------------------------------
  void Clear()
     {
         m_iScore = 0;
         m_iScoreMax = 0;
     }

     //------------------------------------------------------------------------------------------------
  void Insert(SCR_PlayerScoreInfoFiringRange playerScoreInfoFiringRange)
     {
         m_iScore = playerScoreInfoFiringRange.m_iScore;
     }

     //------------------------------------------------------------------------------------------------
  int GetScore(SCR_FiringRangeScoringComponent scoringSystemFiringRange)
     {
         if (!scoringSystemFiringRange)
             return 0;

         return m_iScore;
     }

     //------------------------------------------------------------------------------------------------
  int GetScoreMax()
     {
         return m_iScoreMax;
     }

     //------------------------------------------------------------------------------------------------
     int GetScore()
     {
         return m_iScore;
     }

     //------------------------------------------------------------------------------------------------
     int GetPlayerID()
     {
         return m_iID;
     }
 };