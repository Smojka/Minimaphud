 [BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_BrowserStateUIInfo")]
 class SCR_EditorContentBrowserSaveStateDataUIHQ : SCR_EditorContentBrowserSaveStateDataUI
 {
     //------------------------------------------------------------------------------------------------
  override bool CanBeShown()
     {
         return !SCR_GameModeCampaign.Cast(GetGame().GetGameMode());
     }
 }