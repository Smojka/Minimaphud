 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_CharacterHasWeaponCondition : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         bool result = data.GetCurrentWeaponEntity() != null;
         return GetReturnResult(result);
     }
 };