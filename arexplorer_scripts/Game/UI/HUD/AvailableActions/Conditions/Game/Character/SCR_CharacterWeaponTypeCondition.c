 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_CharacterWeaponTypeCondition : SCR_AvailableActionCondition
 {
     [Attribute("", UIWidgets.EditBox, "", "")]
     private string m_sValue;

     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         bool result = data.GetCurrentWeaponEntity() != null;

         if (result)
             result = data.GetCurrentWeapon().GetWeaponSlotType() == m_sValue;

         return GetReturnResult(result);
     }
 };