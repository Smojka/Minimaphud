 class SCR_EngineHitZoneInfo : SCR_HitZoneInfo
 {
     protected CarControllerComponent m_pCarController;
     protected CarControllerComponent_SA m_pCarController_SA;

     //------------------------------------------------------------------------------------------------
  override EVehicleInfoState GetState()
     {
         EVehicleInfoState state = super.GetState();

         if(GetGame().GetIsClientAuthority())
         {
             if (!m_pCarController)
                 return state;

             if (m_pCarController.GetEngineDrowned())
                 state = EVehicleInfoState.ERROR;

             VehicleWheeledSimulation simulation = m_pCarController.GetSimulation();
             if (state == EVehicleInfoState.ERROR && simulation && simulation.GetThrottle() > 0.1)
                 m_bIsBlinking = true;

             return state;
         }
         else
         {
             if (!m_pCarController_SA)
                 return state;

             if (m_pCarController_SA.GetEngineDrowned())
                 state = EVehicleInfoState.ERROR;

             VehicleWheeledSimulation_SA simulation = m_pCarController_SA.GetSimulation();
             if (state == EVehicleInfoState.ERROR && simulation && simulation.GetThrottle() > 0.1)
                 m_bIsBlinking = true;

             return state;
         }
     }

     //------------------------------------------------------------------------------------------------
  override void DisplayInit(IEntity owner)
     {
         super.DisplayInit(owner);

         if(GetGame().GetIsClientAuthority())
             m_pCarController = CarControllerComponent.Cast(owner.FindComponent(CarControllerComponent));
         else
             m_pCarController_SA = CarControllerComponent_SA.Cast(owner.FindComponent(CarControllerComponent_SA));
     }
 };