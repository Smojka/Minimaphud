 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(EEditableEntityBudget, "m_iBudget")]
 //------------------------------------------------------------------------------------------------
 class SCR_CampaignBuildingCooldownBudgetToEvaluateData : SCR_CampaignBuildingBudgetToEvaluateData
 {
     //------------------------------------------------------------------------------------------------
  int CooldownTimeModifier(int playerId, int originalCooldown)
     {
         return originalCooldown;
     }
 }