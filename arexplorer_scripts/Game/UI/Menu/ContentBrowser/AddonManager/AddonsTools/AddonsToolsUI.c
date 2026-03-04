
 enum SCR_EModPresetsDialogTabs
 {
     PRESETS,
     EXPORT_JSON,
     EXPORT_CLI
 }

 class AddonsToolsUI : SCR_TabDialog
 {
     //------------------------------------------------------------------------------------------------
     override void OnMenuOpen(SCR_ConfigurableDialogUiPreset preset)
     {
         super.OnMenuOpen(preset);

         // Setup workshop button
         SCR_InputButtonComponent workshopButton = FindButton("NavOpenWorkshop");

         if (workshopButton)
         {
             workshopButton.SetVisible(!IsAddonManagerOpened());
             workshopButton.m_OnActivated.Insert(OnOpenWorkshopButton);
         }

         // Mod enabled callback
         EnableExportTabs();
         SCR_AddonManager.GetInstance().m_OnAddonsEnabledChanged.Insert(EnableExportTabs);
     }

     //------------------------------------------------------------------------------------------------
     override void OnMenuClose()
     {
         super.OnMenuClose();

         SCR_AddonManager.GetInstance().m_OnAddonsEnabledChanged.Remove(EnableExportTabs);
     }

     //------------------------------------------------------------------------------------------------
  protected bool IsAddonManagerOpened()
     {
         // Workshop opened
         ContentBrowserUI workshopUI = ContentBrowserUI.Cast(GetGame().GetMenuManager().GetTopMenu());

         if (!workshopUI)
             return false;

         return SCR_WorkshopListAddonsSubmenu.Cast(workshopUI.GetOpenedSubMenu());
     }

     //------------------------------------------------------------------------------------------------
     protected void OnOpenWorkshopButton()
     {
         SCR_MenuToolsLib.GetEventOnAllMenuClosed().Insert(AllMenuClosed);
         SCR_MenuToolsLib.CloseAllMenus({MainMenuUI, ContentBrowserUI});
     }

     //------------------------------------------------------------------------------------------------
     override protected void OnCancel()
     {
         if (!GetGame().GetInputManager().IsContextActive("MenuTextEditContext"))
             super.OnCancel();
     }

     //------------------------------------------------------------------------------------------------
  protected void AllMenuClosed()
     {
         SCR_MenuToolsLib.GetEventOnAllMenuClosed().Remove(AllMenuClosed);
         Close();

         ContentBrowserUI workshopUI = ContentBrowserUI.Cast(GetGame().GetMenuManager().GetTopMenu());
         if (!workshopUI)
             workshopUI = ContentBrowserUI.Cast(GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.ContentBrowser));

         // Open tab
         if (workshopUI)
             workshopUI.OpenModManager();
     }

     //------------------------------------------------------------------------------------------------
  protected void EnableExportTabs()
     {
         SCR_TabViewComponent tabView = m_SuperMenuComponent.GetTabView();
         if (!tabView)
             return;

         if (GetGame().IsPlatformGameConsole())
         {
             tabView.SetTabVisible(SCR_EModPresetsDialogTabs.EXPORT_JSON, false, false);
             tabView.SetTabVisible(SCR_EModPresetsDialogTabs.EXPORT_CLI, false, false);
             tabView.SetPagingButtonsVisible(false, false);
             return;
         }

         bool enable = (SCR_AddonManager.GetInstance().CountOfEnabledAddons() > 0);

         tabView.EnableTab(SCR_EModPresetsDialogTabs.EXPORT_JSON, enable);
         tabView.EnableTab(SCR_EModPresetsDialogTabs.EXPORT_CLI, enable);
     }
 }