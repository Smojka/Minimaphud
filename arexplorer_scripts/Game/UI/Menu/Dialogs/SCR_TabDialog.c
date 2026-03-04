
 class SCR_TabDialog : SCR_ConfigurableDialogUi
 {
     protected SCR_SuperMenuComponent m_SuperMenuComponent;

     //------------------------------------------------------------------------------------------------
     override void OnMenuOpen(SCR_ConfigurableDialogUiPreset preset)
     {
         super.OnMenuOpen(preset);

         m_SuperMenuComponent = SCR_SuperMenuComponent.FindComponent(GetContentLayoutRoot(GetRootWidget()));
         if (!m_SuperMenuComponent)
         {
             Print("SCR_TabDialog | OnMenuOpen() | No SCR_SuperMenuComponent in content layout root", LogLevel.ERROR);
             return;
         }

         m_SuperMenuComponent.Init(GetRootWidget());
     }

     //------------------------------------------------------------------------------------------------
     override void OnMenuShow()
     {
         super.OnMenuShow();

         if (m_SuperMenuComponent)
             m_SuperMenuComponent.OnMenuShow();
     }

     //------------------------------------------------------------------------------------------------
     override void OnMenuHide()
     {
         super.OnMenuHide();

         if (m_SuperMenuComponent)
             m_SuperMenuComponent.OnMenuHide();
     }

     //------------------------------------------------------------------------------------------------
     override void OnMenuUpdate(float tDelta)
     {
         super.OnMenuUpdate(tDelta);

         if (m_SuperMenuComponent)
             m_SuperMenuComponent.OnMenuUpdate(tDelta);
     }

     //------------------------------------------------------------------------------------------------
     override void OnMenuClose()
     {
         super.OnMenuClose();

         if (m_SuperMenuComponent)
             m_SuperMenuComponent.OnMenuClose();
     }

     //----------------------------------------------------------------------------------------
     override void OnMenuFocusGained()
     {
         super.OnMenuFocusGained();

         if (m_SuperMenuComponent)
             m_SuperMenuComponent.OnMenuFocusGained();
     }

     //----------------------------------------------------------------------------------------
     override void OnMenuFocusLost()
     {
         super.OnMenuFocusLost();

         if (m_SuperMenuComponent)
             m_SuperMenuComponent.OnMenuFocusLost();
     }
 }