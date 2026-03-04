 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_WeaponMuzzleChamberedCondition : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         BaseMuzzleComponent muzzle = data.GetCurrentMuzzle();
         if (!muzzle)
             return false;

         auto result = muzzle.IsCurrentBarrelChambered();

         return GetReturnResult(result);
     }
 };