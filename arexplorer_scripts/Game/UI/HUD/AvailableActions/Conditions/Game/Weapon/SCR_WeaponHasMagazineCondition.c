 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_WeaponHasMagazineCondition : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         bool result = false;

         BaseMagazineComponent magazine = data.GetCurrentMagazine();
         if (magazine)
             result = true;

         return GetReturnResult(result);
     }
 };