
 [BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
 class SCR_PlayerBudgetPropEditorAttribute: SCR_BasePlayerBudgetEditorAttribute
 {
     override EEditableEntityBudget GetBudgetType()
     {
         return EEditableEntityBudget.PROPS;
     }
 };