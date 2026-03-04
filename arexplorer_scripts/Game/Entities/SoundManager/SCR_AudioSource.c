 class SCR_AudioSource
 {
     AudioHandle m_AudioHandle = AudioHandle.Invalid;
     IEntity m_Owner;
     protected vector m_aMat[4];
     ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration;
     protected ref array<string> m_aSignalName;
     protected ref array<float> m_aSignalValue;
     ref SCR_InteriorRequestCallback m_InteriorRequestCallback;
     bool m_bTerminated = false;

     static const string INTERIOR_SIGNAL_NAME = "Interior";
     static const string SURFACE_SIGNAL_NAME = "Surface";
     static const string ENTITY_SIZE_SIGNAL_NAME = "EntitySize";
     static const string PHASES_TO_DESTROYED_PHASE_SIGNAL_NAME = "PhasesToDestroyed";
     static const string COLLISION_D_V_SIGNAL_NAME = "CollisionDV";
     static const string DISTANCE_SINAL_NAME = "Distance";
     static const string ROOM_SIZE_SIGNAL_NAME = "RoomSize";

     //------------------------------------------------------------------------------------------------
  void SetSignalValue(string name, float value)
     {
         if (!m_aSignalName)
             m_aSignalName = {};

         if (!m_aSignalValue)
             m_aSignalValue = {};

         m_aSignalName.Insert(name);
         m_aSignalValue.Insert(value);
     }

     //------------------------------------------------------------------------------------------------
  void SetGlobalOcclusionSignals()
     {
         GameSignalsManager gameSignalsManager = GetGame().GetSignalsManager();
         SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();

         SetSignalValue(SCR_SoundManagerEntity.G_INTERIOR_SIGNAL_NAME, gameSignalsManager.GetSignalValue(soundManagerEntity.GetGInteriorSignalIdx()));
         SetSignalValue(SCR_SoundManagerEntity.G_CURR_VEHICLE_COVERAGE_SIGNAL_NAME, gameSignalsManager.GetSignalValue(soundManagerEntity.GetGCurrVehicleCoverageSignalIdx()));
         SetSignalValue(SCR_SoundManagerEntity.G_IS_THIRD_PERSON_CAM_SIGNAL_NAME, gameSignalsManager.GetSignalValue(soundManagerEntity.GetGIsThirdPersonCamSignalIdx()));
         SetSignalValue(SCR_SoundManagerEntity.G_ROOM_SIZE, gameSignalsManager.GetSignalValue(soundManagerEntity.GetRoomSizeIdx()));
     }

     //------------------------------------------------------------------------------------------------
     bool Play()
     {
         // Play event
         m_AudioHandle = AudioSystem.PlayEvent(m_AudioSourceConfiguration.m_sSoundProject, m_AudioSourceConfiguration.m_sSoundEventName, m_aMat, m_aSignalName, m_aSignalValue);

         // Check if AudioHandle is valid
         return m_AudioHandle != AudioHandle.Invalid;
     }

     //------------------------------------------------------------------------------------------------
     void SetTransformation(vector mat[4])
     {
         m_aMat = mat;
     }

     //------------------------------------------------------------------------------------------------
     void UpdateTransformation()
     {
         m_Owner.GetTransform(m_aMat);
         AudioSystem.SetSoundTransformation(m_AudioHandle, m_aMat);
     }

     //------------------------------------------------------------------------------------------------
     void Ternimate()
     {
         AudioSystem.TerminateSound(m_AudioHandle);
         m_bTerminated = true;
     }

     //------------------------------------------------------------------------------------------------
     void SCR_AudioSource(IEntity owner, SCR_AudioSourceConfiguration audioSourceConfiguration, float distance)
     {
         m_AudioSourceConfiguration = audioSourceConfiguration;
         m_Owner = owner;

         // Set distance signal
         if (SCR_Enum.HasFlag(audioSourceConfiguration.m_eFlags, EAudioSourceConfigurationFlag.DistanceSignal))
             SetSignalValue(DISTANCE_SINAL_NAME, distance);
     }
 }