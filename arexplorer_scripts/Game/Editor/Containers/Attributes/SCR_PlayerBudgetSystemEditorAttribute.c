
 [BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
 class SCR_PlayerBudgetSystemEditorAttribute: SCR_BasePlayerBudgetEditorAttribute
 {
     override EEditableEntityBudget GetBudgetType()
     {
         return EEditableEntityBudget.SYSTEMS;
     }
 };