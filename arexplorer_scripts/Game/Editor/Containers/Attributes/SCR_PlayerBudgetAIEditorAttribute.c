
 [BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
 class SCR_PlayerBudgetAIEditorAttribute: SCR_BasePlayerBudgetEditorAttribute
 {
     override EEditableEntityBudget GetBudgetType()
     {
         return EEditableEntityBudget.AI;
     }
 };