
 void ScriptInvokerActionMethod(string name, float multiplier);
 typedef func ScriptInvokerActionMethod;
 typedef ScriptInvokerBase<ScriptInvokerActionMethod> ScriptInvokerAction;

 //------------------------------------------------------------------------------------------------
 class SCR_MenuActionsComponent : SCR_ScriptedWidgetComponent
 {
     [Attribute(desc: "Actions to be shown in the Tooltip")]
     protected ref array<ref SCR_MenuActionPreset> m_aActions;

     [Attribute("400", desc: "Action listeners activation delay")]
     protected int m_iDelay;

     protected bool m_bHasActionListeners;

     protected ref ScriptInvokerAction m_OnAction;

     //------------------------------------------------------------------------------------------------
     override void HandlerAttached(Widget w)
     {
         super.HandlerAttached(w);

         AddActionListenersDelayed(m_iDelay);

         SCR_MenuHelper().GetOnMenuFocusLost().Insert(OnMenuDeactivated);
         SCR_MenuHelper().GetOnMenuHide().Insert(OnMenuDeactivated);
         SCR_MenuHelper().GetOnMenuClose().Insert(OnMenuDeactivated);
     }

     //------------------------------------------------------------------------------------------------
     override void HandlerDeattached(Widget w)
     {
         super.HandlerDeattached(w);

         RemoveActionListeners();

         SCR_MenuHelper().GetOnMenuFocusLost().Remove(OnMenuDeactivated);
         SCR_MenuHelper().GetOnMenuHide().Remove(OnMenuDeactivated);
         SCR_MenuHelper().GetOnMenuClose().Remove(OnMenuDeactivated);
     }

     //------------------------------------------------------------------------------------------------
     // Owner Menu Events
     //------------------------------------------------------------------------------------------------
     //------------------------------------------------------------------------------------------------
     protected void OnMenuDeactivated(ChimeraMenuBase menu)
     {
         if (menu == ChimeraMenuBase.GetOwnerMenu(m_wRoot))
             RemoveActionListeners();
     }

     //------------------------------------------------------------------------------------------------
     // Events
     //------------------------------------------------------------------------------------------------
     //------------------------------------------------------------------------------------------------
  protected void OnAction(float multiplier)
     {
         if (!m_wRoot.IsVisible())
             return;

         InputManager inputManager = GetGame().GetInputManager();
         foreach (SCR_MenuActionPreset action : m_aActions)
         {
             if (inputManager.GetActionTriggered(action.m_sActionName) && m_OnAction)
                 m_OnAction.Invoke(action.m_sActionName, multiplier);
         }
     }

     //------------------------------------------------------------------------------------------------
     // Action Listeners
     //------------------------------------------------------------------------------------------------
     //------------------------------------------------------------------------------------------------
  protected void AddActionListenersDelayed(int delay)
     {
         if (m_bHasActionListeners)
             return;

         GetGame().GetCallqueue().CallLater(AddActionListeners, delay);
     }

     //------------------------------------------------------------------------------------------------
     protected void AddActionListeners()
     {
         GetGame().GetCallqueue().Remove(AddActionListeners);

         if (m_bHasActionListeners)
             return;

         m_bHasActionListeners = true;

         InputManager inputManager = GetGame().GetInputManager();
         string name;
         EActionTrigger trigger;
         foreach (SCR_MenuActionPreset action : m_aActions)
         {
             name = action.m_sActionName;
             trigger = action.m_eActionTrigger;
             inputManager.AddActionListener(name, trigger, OnAction);
         }
     }

     //------------------------------------------------------------------------------------------------
     protected void RemoveActionListeners()
     {
         GetGame().GetCallqueue().Remove(AddActionListeners);

         if (!m_bHasActionListeners)
             return;

         m_bHasActionListeners = false;

         InputManager inputManager = GetGame().GetInputManager();
         string name;
         EActionTrigger trigger;
         foreach (SCR_MenuActionPreset action : m_aActions)
         {
             name = action.m_sActionName;
             trigger = action.m_eActionTrigger;
             inputManager.RemoveActionListener(name, trigger, OnAction);
         }
     }

     //------------------------------------------------------------------------------------------------
     void ActivateActions()
     {
         AddActionListenersDelayed(m_iDelay);
     }

     //------------------------------------------------------------------------------------------------
     void DeactivateActions()
     {
         RemoveActionListeners();
     }

     //------------------------------------------------------------------------------------------------
     // Utility
     //------------------------------------------------------------------------------------------------
     //------------------------------------------------------------------------------------------------
     array<ref SCR_MenuActionPreset> GetActions()
     {
         return m_aActions;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvokerAction GetOnAction()
     {
         if (!m_OnAction)
             m_OnAction = new ScriptInvokerAction();

         return m_OnAction;
     }

     //------------------------------------------------------------------------------------------------
     static SCR_MenuActionsComponent FindComponent(Widget w)
     {
         return SCR_MenuActionsComponent.Cast(w.FindHandler(SCR_MenuActionsComponent));
     }
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(), SCR_BaseContainerCustomTitleField("m_sActionName")]
 class SCR_MenuActionPreset
 {
     [Attribute()]
     string m_sActionName;

     [Attribute("4", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EActionTrigger))]
     EActionTrigger m_eActionTrigger;
 }
