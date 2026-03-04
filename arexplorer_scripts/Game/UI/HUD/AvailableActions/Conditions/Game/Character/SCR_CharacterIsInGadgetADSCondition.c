 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_CharacterIsInGadgetADSCondition : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         bool result = data.GetGadgetRaised();

         return GetReturnResult(result);
     }
 };