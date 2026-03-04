 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_ANDCondition : SCR_AvailableActionsGroupCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         bool isOk = true;
         foreach (auto subCondition : m_aConditions)
         {
             if (subCondition && !subCondition.IsAvailable(data))
             {
                 isOk = false;
                 break;
             }
         }

         return GetReturnResult(isOk);
     }
 };