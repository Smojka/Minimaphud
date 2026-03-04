 [BaseContainerProps()]
 class SCR_AvailableActionCondition
 {
     [Attribute("1", UIWidgets.CheckBox)]
     protected bool m_bEnabled;

     [Attribute("0", UIWidgets.CheckBox)]
     protected bool m_bNegateCondition;

     //------------------------------------------------------------------------------------------------
  bool IsEnabled()
     {
         return m_bEnabled;
     }

     //------------------------------------------------------------------------------------------------
  protected bool GetReturnResult(bool desiredResult)
     {
         if (m_bNegateCondition)
             return !desiredResult;

         return desiredResult;
     }

     //------------------------------------------------------------------------------------------------
  bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         return GetReturnResult(true);
     }
 }