 class SCR_RespawnTimer
 {
     protected WorldTimestamp m_fStartTime;
     protected float m_fDuration;

     void Start(WorldTimestamp timeNow)
     {
         m_fStartTime = timeNow;
     }

     WorldTimestamp GetStartTime()
     {
         return m_fStartTime;
     }

     bool IsFinished(WorldTimestamp timeNow, float additionalTime = 0)
     {
         return timeNow.GreaterEqual(m_fStartTime.PlusSeconds(m_fDuration + additionalTime));
     }

     float GetRemainingTime(WorldTimestamp timeNow, float additionalTime = 0)
     {
         WorldTimestamp endTime = m_fStartTime.PlusSeconds(m_fDuration + additionalTime);
         float rem = endTime.DiffMilliseconds(timeNow);
         if (rem <= 0)
             return 0.0;

         return rem / 1000.0;
     }

     float GetDuration()
     {
         return m_fDuration;
     }

     void SetDuration(float duration)
     {
         m_fDuration = duration;
     }

     bool RplSave(ScriptBitWriter writer)
     {
         writer.Write(m_fStartTime, 64);
         writer.WriteFloat(m_fDuration);

         return true;
     }

     bool RplLoad(ScriptBitReader reader)
     {
         reader.Read(m_fStartTime, 64);
         reader.ReadFloat(m_fDuration);

         return true;
     }

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
     static bool PropCompare(SCR_RespawnTimer prop, SSnapSerializerBase snapshot, ScriptCtx ctx)
     {
         return snapshot.Compare(prop.m_fStartTime, 8)
             && snapshot.Compare(prop.m_fDuration, 4);
     }

     //------------------------------------------------------------------------------------------------
     static bool Extract(SCR_RespawnTimer prop, ScriptCtx ctx, SSnapSerializerBase snapshot)
     {
         snapshot.SerializeBytes(prop.m_fStartTime, 8);
         snapshot.SerializeBytes(prop.m_fDuration, 4);
         return true;
     }

     //------------------------------------------------------------------------------------------------
     static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, SCR_RespawnTimer prop)
     {
         snapshot.SerializeBytes(prop.m_fStartTime, 8);
         snapshot.SerializeBytes(prop.m_fDuration, 4);
         return true;
     }
 };