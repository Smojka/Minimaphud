 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(EEditableEntityBudget, "m_iBudget")]
 class SCR_CampaignBuildingRankBudgetToEvaluateData : SCR_CampaignBuildingBudgetToEvaluateData
 {
     override bool CanBeUsed()
     {
         BaseGameMode gameMode = GetGame().GetGameMode();
         if (!gameMode)
             return false;

         return SCR_XPHandlerComponent.Cast(gameMode.FindComponent(SCR_XPHandlerComponent));
     }
 }