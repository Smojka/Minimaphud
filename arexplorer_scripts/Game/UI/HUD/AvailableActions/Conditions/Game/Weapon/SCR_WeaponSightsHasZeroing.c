 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_WeaponSightHasZeroingCondition : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         BaseWeaponComponent currentweapon = data.GetCurrentWeapon();
         if (!currentweapon)
             return false;

         float zeroing = currentweapon.GetCurrentSightsZeroing();
         return GetReturnResult(zeroing > 0);
     }
 };