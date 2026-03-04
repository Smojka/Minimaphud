
 [BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
 class SCR_PlayerBudgetVehicleEditorAttribute: SCR_BasePlayerBudgetEditorAttribute
 {
     override EEditableEntityBudget GetBudgetType()
     {
         return EEditableEntityBudget.VEHICLES;
     }
 };