 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_UIInfoSpawnRequestResult : SCR_UIName
 {
     [Attribute("#AR-DeployMenu_DeployIn", desc: "Text with respawn timer as %1", uiwidget: UIWidgets.LocaleEditBox)]
     protected LocalizedString m_sNameWithTimer;

     //------------------------------------------------------------------------------------------------
  LocalizedString GetNameWithTimer()
     {
         return m_sNameWithTimer;
     }
 }