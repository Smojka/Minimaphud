 [BaseContainerProps()]
 class SCR_AudioHandleLoop
 {
     AudioHandle m_AudioHandle = AudioHandle.Invalid;

     vector m_aMat[4];

     string m_sSoundEvent;

     //------------------------------------------------------------------------------------------------
     // destructor
     void ~SCR_AudioHandleLoop()
     {
         AudioSystem.TerminateSound(m_AudioHandle);
     }
 }