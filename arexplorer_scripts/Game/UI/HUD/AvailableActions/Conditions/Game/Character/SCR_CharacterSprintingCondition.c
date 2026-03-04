 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_CharacterSprintingCondition : SCR_AvailableActionCondition
 {
     [Attribute("0", UIWidgets.EditBox, "Minimum temporary sprinting time. Set to below zero to track persistent sprinting only.", "")]
     protected float m_fMinimumTime;

     //------------------------------------------------------------------------------------------------
     override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         bool result = data.GetIsCharacterSprinting();
         if (m_fMinimumTime > 0)
             result = result && data.GetCharacterSprintingTime() > m_fMinimumTime;
         else if (m_fMinimumTime < 0)
             result = result && data.GetCharacterSprintingTime() == 0;

         return GetReturnResult(result);
     }
 };