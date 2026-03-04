 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_VehicleClutchCondition : SCR_AvailableActionCondition
 {
     [Attribute(defvalue: "0.05", uiwidget: UIWidgets.Auto, desc: "Minimum clutch to pass this conditon")]
     protected float m_fMinimumClutch;

     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         if(GetGame().GetIsClientAuthority())
         {
             CarControllerComponent controller = CarControllerComponent.Cast(data.GetCurrentVehicleController());
             if (!controller)
                 return false;

             VehicleWheeledSimulation simulation = VehicleWheeledSimulation.Cast(controller.GetSimulation());
             if (!simulation)
                 return false;

             bool result = simulation.GetClutch() >= m_fMinimumClutch;

             return GetReturnResult(result);
         }
         else
         {
             CarControllerComponent_SA controller = CarControllerComponent_SA.Cast(data.GetCurrentVehicleController());
             if (!controller)
                 return false;

             VehicleWheeledSimulation_SA simulation = VehicleWheeledSimulation_SA.Cast(controller.GetSimulation());
             if (!simulation)
                 return false;

             bool result = simulation.GetClutch() >= m_fMinimumClutch;

             return GetReturnResult(result);
         }

     }
 };