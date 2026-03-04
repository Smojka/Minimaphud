 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(configRoot: true)]
 class SCR_BaseSelectionMenu : ScriptedSelectionMenu
 {
     #ifdef ENABLE_DIAG
     protected static bool s_bEnableRadialMenuLog = false;
     protected static bool s_bEnableRadialMenuDiag = false;
     protected static bool s_bDiagnosticsMenuRegistered = false;
     #endif

     [Attribute("", UIWidgets.Object)]
     protected ref SCR_BaseSelectionMenuDisplay m_pDisplay;

     //------------------------------------------------------------------------------------------------
  protected override event void OnOpen(IEntity owner)
     {
         // TODO: Handle creation of widgets
         #ifdef ENABLE_DIAG
         if (SCR_BaseSelectionMenu.IsRadialMenuLoggingEnabled())
         {
             Print("Radial menu " + this + " opened! Owner: " + owner);
         }
         #endif
     }

     //------------------------------------------------------------------------------------------------
  protected override event void OnClose(IEntity owner)
     {
         // TODO: Handle destruction of widgets
         #ifdef ENABLE_DIAG
         if (SCR_BaseSelectionMenu.IsRadialMenuLoggingEnabled())
         {
             Print("Radial menu " + this + " closed! Owner: " + owner);
         }
         #endif
     }

     //------------------------------------------------------------------------------------------------
  protected override event void OnUpdate(IEntity owner, float timeSlice)
     {
         // TODO: Handle update of widgets
         // TODO: Handle input, selection and performing of the action
     }

     //------------------------------------------------------------------------------------------------
     // Initialize the radial menu
     void SCR_BaseSelectionMenu()
     {
         #ifdef ENABLE_DIAG
         if (!s_bDiagnosticsMenuRegistered)
         {
             const string radialMenuGroup = "Radial Menu";
             DiagMenu.RegisterMenu(SCR_DebugMenuID.DEBUGUI_RADIALMENU_MENU, radialMenuGroup, "");
             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_RADIALMENU_DIAGNOSTICS, "", "Enable diagnostics", radialMenuGroup);
             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_RADIALMENU_LOGGING, "", "Enable logging", radialMenuGroup);
             s_bDiagnosticsMenuRegistered = true;
         }
         #endif

     }

     //------------------------------------------------------------------------------------------------
     // Cleanup
     void ~SCR_BaseSelectionMenu()
     {
         m_pDisplay = null;
     }


     #ifdef ENABLE_DIAG
     //------------------------------------------------------------------------------------------------
  static bool IsRadialMenuDiagEnabled()
     {
         s_bEnableRadialMenuDiag = DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_RADIALMENU_DIAGNOSTICS);
         return s_bEnableRadialMenuDiag;
     }

     //------------------------------------------------------------------------------------------------
  static bool IsRadialMenuLoggingEnabled()
     {
         s_bEnableRadialMenuLog = DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_RADIALMENU_LOGGING);
         return s_bEnableRadialMenuLog;
     }
     #endif
 };