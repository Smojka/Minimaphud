 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_CharacterCanGetOutCondition : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         return GetReturnResult(data.GetCanCharacterGetOutVehicle());
     }
 };