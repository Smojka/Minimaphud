 void ScriptInvokerActiveWidgetInteractionMethod(bool isActive, int delay);
 typedef func ScriptInvokerActiveWidgetInteractionMethod;
 typedef ScriptInvokerBase<ScriptInvokerActiveWidgetInteractionMethod> ScriptInvokerActiveWidgetInteraction;

 void ScriptInvokerDialogMethod(DialogUI dialog);
 typedef func ScriptInvokerDialogMethod;
 typedef ScriptInvokerBase<ScriptInvokerDialogMethod> ScriptInvokerDialog;

 void ScriptInvokerMenuMethod(ChimeraMenuBase menu);
 typedef func ScriptInvokerMenuMethod;
 typedef ScriptInvokerBase<ScriptInvokerMenuMethod> ScriptInvokerMenu;

 //------------------------------------------------------------------------------------------------
 class SCR_MenuHelper
 {
     protected static ref ScriptInvokerActiveWidgetInteraction m_OnActiveWidgetInteraction;
     protected static ref ScriptInvokerDialog m_OnDialogOpen;
     protected static ref ScriptInvokerDialog m_OnDialogClose;
     protected static ref ScriptInvokerMenu m_OnMenuOpen;
     protected static ref ScriptInvokerMenu m_OnMenuOpened;
     protected static ref ScriptInvokerMenu m_OnMenuClose;
     protected static ref ScriptInvokerMenu m_OnMenuFocusGained;
     protected static ref ScriptInvokerMenu m_OnMenuFocusLost;
     protected static ref ScriptInvokerMenu m_OnMenuShow;
     protected static ref ScriptInvokerMenu m_OnMenuHide;
     protected static ref ScriptInvokerMenu m_OnTabChange;

     // ---- Invokers ----
     //------------------------------------------------------------------------------------------------
     static ScriptInvokerActiveWidgetInteraction GetOnActiveWidgetInteraction()
     {
         if (!m_OnActiveWidgetInteraction)
             m_OnActiveWidgetInteraction = new ScriptInvokerActiveWidgetInteraction();

         return m_OnActiveWidgetInteraction;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerDialog GetOnDialogOpen()
     {
         if (!m_OnDialogOpen)
             m_OnDialogOpen = new ScriptInvokerDialog();

         return m_OnDialogOpen;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerDialog GetOnDialogClose()
     {
         if (!m_OnDialogClose)
             m_OnDialogClose = new ScriptInvokerDialog();

         return m_OnDialogClose;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerMenu GetOnMenuOpen()
     {
         if (!m_OnMenuOpen)
             m_OnMenuOpen = new ScriptInvokerMenu();

         return m_OnMenuOpen;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerMenu GetOnMenuOpened()
     {
         if (!m_OnMenuOpened)
             m_OnMenuOpened = new ScriptInvokerMenu();

         return m_OnMenuOpened;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerMenu GetOnMenuClose()
     {
         if (!m_OnMenuClose)
             m_OnMenuClose = new ScriptInvokerMenu();

         return m_OnMenuClose;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerMenu GetOnMenuFocusGained()
     {
         if (!m_OnMenuFocusGained)
             m_OnMenuFocusGained = new ScriptInvokerMenu();

         return m_OnMenuFocusGained;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerMenu GetOnMenuFocusLost()
     {
         if (!m_OnMenuFocusLost)
             m_OnMenuFocusLost = new ScriptInvokerMenu();

         return m_OnMenuFocusLost;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerMenu GetOnMenuShow()
     {
         if (!m_OnMenuShow)
             m_OnMenuShow = new ScriptInvokerMenu();

         return m_OnMenuShow;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerMenu GetOnMenuHide()
     {
         if (!m_OnMenuHide)
             m_OnMenuHide = new ScriptInvokerMenu();

         return m_OnMenuHide;
     }

     //------------------------------------------------------------------------------------------------
     static ScriptInvokerMenu GetOnTabChange()
     {
         if (!m_OnTabChange)
             m_OnTabChange = new ScriptInvokerMenu();

         return m_OnTabChange;
     }

     // ---- Methods ----
     //------------------------------------------------------------------------------------------------
  static void SetActiveWidgetInteractionState(bool isActive, int delay = 0)
     {
         if (m_OnActiveWidgetInteraction)
             m_OnActiveWidgetInteraction.Invoke(isActive, delay);
     }

     //------------------------------------------------------------------------------------------------
  static bool IsInTopMenu(notnull Widget componentRoot)
     {
         MenuManager menuManager = GetGame().GetMenuManager();
         if (!menuManager)
             return false;

         return menuManager.GetOwnerMenu(componentRoot) == menuManager.GetTopMenu();
     }

     // -- Calls --
     //------------------------------------------------------------------------------------------------
  static void OnDialogOpen(DialogUI dialog)
     {
         if (m_OnDialogOpen)
             m_OnDialogOpen.Invoke(dialog);
     }

     //------------------------------------------------------------------------------------------------
  static void OnDialogClose(DialogUI dialog)
     {
         if (m_OnDialogClose)
             m_OnDialogClose.Invoke(dialog);
     }

     //------------------------------------------------------------------------------------------------
  static void OnMenuOpen(ChimeraMenuBase menu)
     {
         if (m_OnMenuOpen)
             m_OnMenuOpen.Invoke(menu);
     }

     //------------------------------------------------------------------------------------------------
  static void OnMenuOpened(ChimeraMenuBase menu)
     {
         if (m_OnMenuOpened)
             m_OnMenuOpened.Invoke(menu);
     }

     //------------------------------------------------------------------------------------------------
  static void OnMenuClose(ChimeraMenuBase menu)
     {
         if (m_OnMenuClose)
             m_OnMenuClose.Invoke(menu);
     }

     //------------------------------------------------------------------------------------------------
  static void OnMenuFocusGained(ChimeraMenuBase menu)
     {
         if (m_OnMenuFocusGained)
             m_OnMenuFocusGained.Invoke(menu);
     }

     //------------------------------------------------------------------------------------------------
  static void OnMenuFocusLost(ChimeraMenuBase menu)
     {
         if (m_OnMenuFocusLost)
             m_OnMenuFocusLost.Invoke(menu);
     }

     //------------------------------------------------------------------------------------------------
  static void OnMenuShow(ChimeraMenuBase menu)
     {
         if (m_OnMenuShow)
             m_OnMenuShow.Invoke(menu);
     }

     //------------------------------------------------------------------------------------------------
  static void OnMenuHide(ChimeraMenuBase menu)
     {
         if (m_OnMenuHide)
             m_OnMenuHide.Invoke(menu);
     }

     //------------------------------------------------------------------------------------------------
  static void OnTabChange(ChimeraMenuBase menu)
     {
         if (m_OnTabChange)
             m_OnTabChange.Invoke(menu);
     }
 }

 //------------------------------------------------------------------------------------------------
 enum SCR_EListMenuWidgetFocus
 {
     LIST = 0,
     FILTERING = 1,
     SORTING = 2,
     FALLBACK = 3,
     NULL = 4
 }