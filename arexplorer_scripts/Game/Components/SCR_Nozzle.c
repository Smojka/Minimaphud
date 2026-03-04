 class SCR_Nozzle
 {
     private int m_iProviderIndex = -1;

     //------------------------------------------------------------------------------------------------
  void SetProvider(int iProviderIndex)
     {
         m_iProviderIndex = iProviderIndex;
     }

     //------------------------------------------------------------------------------------------------
  int GetProvider()
     {
         return m_iProviderIndex;
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_Nozzle(int iProviderIndex)
     {
         SetProvider(iProviderIndex);
     }
 }