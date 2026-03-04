 [BaseContainerProps(configRoot: true)]
 class SCR_InsectParticle
 {
     SCR_AudioHandleLoop m_AudioHandle;

     vector m_vPosition;

     int m_iSoundGroup;

     float m_fDensity;

     ParticleEffectEntity m_ParticleEffect;

     IEntity m_InsectEntity;

 #ifdef ENABLE_DIAG
     ref Shape m_DebugShape;
 #endif

     //------------------------------------------------------------------------------------------------
     // destructor
     void ~SCR_InsectParticle()
     {
         delete m_ParticleEffect;
         delete m_InsectEntity;
     }
 }