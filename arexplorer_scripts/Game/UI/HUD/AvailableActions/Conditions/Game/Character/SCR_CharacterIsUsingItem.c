 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_CharacterIsUsingItem : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         bool result = data.GetIsCharacterUsingItem();
         return GetReturnResult(result);
     }
 };