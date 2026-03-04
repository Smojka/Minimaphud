 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_ORCondition : SCR_AvailableActionsGroupCondition
 {
     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         bool isOk = false;
         foreach (auto subCondition : m_aConditions)
         {
             if (subCondition && subCondition.IsAvailable(data))
             {
                 isOk = true;
                 break;
             }
         }

         return GetReturnResult(isOk);
     }
 };