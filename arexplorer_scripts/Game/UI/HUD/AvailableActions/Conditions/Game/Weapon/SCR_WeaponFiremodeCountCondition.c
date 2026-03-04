 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_WeaponFiremodeCountCondition : SCR_AvailableActionCondition
 {
     [Attribute("3", UIWidgets.ComboBox, "Cond operator", "", ParamEnumArray.FromEnum(SCR_ComparerOperator) )]
     private SCR_ComparerOperator m_eOperator;

     [Attribute("2", UIWidgets.EditBox, "Min firemodes, 2 = safety & default usually", "")]
     private int m_iValue;

     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         BaseMuzzleComponent muzzle = data.GetCurrentMuzzle();
         if (!muzzle)
             return false;

         auto result = SCR_Comparer<int>.Compare(m_eOperator, muzzle.GetFireModesCount(), m_iValue);
         return GetReturnResult(result);
     }
 };