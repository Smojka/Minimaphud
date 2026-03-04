 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(EEditableEntityBudget, "m_eBudget")]
 class SCR_CampaignBuildingBudgetToEvaluateData
 {
     [Attribute(desc: "Fill in the budgets to be used with this provider", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityBudget))]
     protected EEditableEntityBudget m_eBudget;

     [Attribute("0", UIWidgets.CheckBox, "If this provider has set: Use master provider and the same budget is set on master provider too, master provider budget will be used to evaluate placement.")]
     protected bool m_bUseMasterProviderBudget;

     [Attribute("0", UIWidgets.CheckBox, "Set if this budget should be used in UI bar. Two budgets can be set per provider.")]
     protected bool m_bShowBudgetInUI;

     //------------------------------------------------------------------------------------------------
  bool CanBeUsed()
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
     EEditableEntityBudget GetBudget()
     {
         return m_eBudget;
     }

     //------------------------------------------------------------------------------------------------
  bool UseMasterProviderBudget()
     {
         return m_bUseMasterProviderBudget;
     }

     //------------------------------------------------------------------------------------------------
  bool CanShowBudgetInUI()
     {
         return m_bShowBudgetInUI;
     }
 }