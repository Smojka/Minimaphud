 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_VehiclePersistentBrakeEngagedCondition : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         if(GetGame().GetIsClientAuthority())
         {
             CarControllerComponent carController = CarControllerComponent.Cast(data.GetCurrentVehicleController());
             if (!carController)
                 return false;

             bool result = carController.GetPersistentHandBrake();
             return GetReturnResult(result);
         }
         else
         {
             CarControllerComponent_SA carController = CarControllerComponent_SA.Cast(data.GetCurrentVehicleController());
             if (!carController)
                 return false;

             bool result = carController.GetPersistentHandBrake();
             return GetReturnResult(result);
         }
     }
 };