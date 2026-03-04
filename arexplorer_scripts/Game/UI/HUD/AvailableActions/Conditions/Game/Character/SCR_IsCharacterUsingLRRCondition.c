 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_IsCharacterUsingLRRCondition : SCR_AvailableActionCondition
 {
     //------------------------------------------------------------------------------------------------
     override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         return GetReturnResult(data.m_bIsRadioLongRange);
     }
 };