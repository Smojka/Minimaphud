 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_IsCharacterInControlCondition : SCR_AvailableActionCondition
 {
     [Attribute("0")]
     protected bool m_bIsAvailableInVehicle;

     //------------------------------------------------------------------------------------------------
     override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         // Can be controlled in vehicle
         bool vehicleControl = !data.GetIsCharacterInVehicle();
         if (m_bIsAvailableInVehicle)
             vehicleControl = true;

         // Can character control action
         bool inControl =    !data.GetIsCharacterFalling() &&
                             !data.GetIsCharacterSwimming() &&
                             vehicleControl;

         return GetReturnResult(inControl);
     }
 };