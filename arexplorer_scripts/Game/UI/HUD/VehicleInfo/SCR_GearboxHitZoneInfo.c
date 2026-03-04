 class SCR_GearboxHitZoneInfo : SCR_HitZoneInfo
 {
     protected CarControllerComponent m_pCarController;
     protected CarControllerComponent_SA m_pCarController_SA;

     //------------------------------------------------------------------------------------------------
  override EVehicleInfoState GetState()
     {
         EVehicleInfoState state = super.GetState();

         if(GetGame().GetIsClientAuthority())
         {
             if (!m_pCarController || m_pCarController.GetCurrentGear() == 1)
                 return state;

             if (state != EVehicleInfoState.ERROR)
                 return state;

             VehicleWheeledSimulation simulation = m_pCarController.GetSimulation();
             if (simulation && simulation.GetSpeedKmh() > 1)
                 m_bIsBlinking = true;

             return state;
         }
         else
         {
             if (!m_pCarController_SA || m_pCarController_SA.GetCurrentGear() == 1)
                 return state;

             if (state != EVehicleInfoState.ERROR)
                 return state;

             VehicleWheeledSimulation_SA simulation = m_pCarController_SA.GetSimulation();
             if (simulation && simulation.GetSpeedKmh() > 1)
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