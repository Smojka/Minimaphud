 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_CharacterAimingSightsDownCondition : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         bool result = data.GetIsCharacterADS();
         return GetReturnResult(result);
     }
 };