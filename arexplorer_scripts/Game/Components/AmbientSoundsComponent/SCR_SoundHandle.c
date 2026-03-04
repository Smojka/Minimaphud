 [BaseContainerProps(configRoot: true)]
 class SCR_SoundHandle
 {
     vector m_aMat[4];

     int m_iSoundGroup;

     int m_iSoundType;

     int m_iSoundDef;

     SCR_SoundDef m_SoundDef;

     ref array<int> m_aRepTime;

     int m_iRepTimeIdx;

     float m_fDensity;

     //------------------------------------------------------------------------------------------------
  void UpdateDensity(float sampleLenght, float worldTime)
     {
         float sequenceLenght = GetSequenceLenght(worldTime);
         if (sequenceLenght == 0)
         {
             m_fDensity = 0;
             return;
         }

         m_fDensity = Math.Clamp(sampleLenght * m_aRepTime.Count() / sequenceLenght, 0, 1) * 100;
     }

     //------------------------------------------------------------------------------------------------
  float GetSequenceLenght(float worldTime)
     {
         int size = m_aRepTime.Count();
         if (size == 0)
             return 0;
         else
             return m_aRepTime[size - 1] - worldTime + m_SoundDef.m_iSampleLength;
     }

     //------------------------------------------------------------------------------------------------
     protected int GetRandomInt(int value, int random)
     {
         if (random == 0)
             return value;

         int min = value - random;
         if (min < 1)
             min = 1;

         return Math.RandomIntInclusive(min, value + random);
     }

     //------------------------------------------------------------------------------------------------
     protected void CreateSequence(array<ref SCR_SequenceDef> sequenceDefinitions, int idx)
     {
         SCR_SequenceDef sequenceDefinition = sequenceDefinitions[idx];

         for (int i = 0, count = GetRandomInt(sequenceDefinition.m_iRepCount, sequenceDefinition.m_iRepCountRnd); i < count; i++)
         {
             if (i > 0)
                 m_aRepTime.Insert(GetRandomInt(sequenceDefinition.m_iRepTime, sequenceDefinition.m_iRepTimeRnd) + m_aRepTime[m_aRepTime.Count() - 1]);

             int idxNew = idx - 1;
             if (idxNew >= 0)
                 CreateSequence(sequenceDefinitions, idxNew);
         }
     }

     //------------------------------------------------------------------------------------------------
  void UpdateRepTime(float gameTime)
     {
         if (m_aRepTime)
             m_aRepTime.Clear();
         else
             m_aRepTime = {};

         m_aRepTime.Insert(gameTime + GetRandomInt(m_SoundDef.m_iStartDelay, m_SoundDef.m_iStartDelayRnd));

         if (m_SoundDef.m_aSequenceDef.Count() == 0)
             Print("AmbientSoundsComponent: " + typename.EnumToString(ESoundName, m_SoundDef.m_eSoundName) + " is missing sequence definition", LogLevel.WARNING);
         else
             CreateSequence(m_SoundDef.m_aSequenceDef, m_SoundDef.m_aSequenceDef.Count() - 1);
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_SoundHandle(int soundGroupIdx, int soundTypeIdx, int soundDefIdx, vector mat[4], array<ref SCR_SoundGroup> soundGroup, float worldTime)
     {
         m_iSoundGroup = soundGroupIdx;
         m_iSoundType = soundTypeIdx;
         m_iSoundDef = soundDefIdx;
         m_aMat = mat;
         m_SoundDef = soundGroup[soundGroupIdx].m_aSoundType[soundTypeIdx].m_aSoundDef[soundDefIdx];
         UpdateRepTime(worldTime);
         UpdateDensity(m_SoundDef.m_iSampleLength, worldTime);
     }
 }