 class SCR_HZInfoEngineHeli : SCR_HZInfoHeli
 {
     //------------------------------------------------------------------------------------------------
  override EVehicleInfoState GetState()
     {
         EVehicleInfoState state = super.GetState();

         if (m_HelicoperController && m_HelicoperController.GetEngineDrowned())
             return EVehicleInfoState.ERROR;

         return state;
     }
 }