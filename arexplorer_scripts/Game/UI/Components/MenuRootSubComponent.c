
 class MenuRootSubComponent: ScriptedWidgetComponent
 {
     private Widget m_Widget;
     private MenuRootComponent m_Root;

     Widget GetWidget()
     {
         return m_Widget;
     }
     MenuRootBase GetMenu()
     {
         if (!m_Root) return null;
         return m_Root.GetMenu();
     }
     MenuRootComponent GetRootComponent()
     {
         return m_Root;
     }

     protected bool IsUnique()
     {
         return true;
     }

     void HandlerAttachedScripted(Widget w);

     override void HandlerAttached(Widget w)
     {
         if (SCR_Global.IsEditMode()) return; //--- Run-time only
         m_Widget = w;

         m_Root = MenuRootComponent.GetRootOf(m_Widget);
         if (IsUnique())
         {
             if (!m_Root.FindComponent(Type()))
             {
                 m_Root.AddComponent(this);
             }
             else
             {
                 Print(string.Format("Duplicate instance of UI component %1 found on widget '%2'! The component is marked as unique, only one instance is allowed.", Type(), w.GetName()), LogLevel.WARNING);
                 return;
             }
         }

         if (m_Root.GetMenu())
         {
             //--- Execute after delay to make sure all new components are registered first
             GetGame().GetCallqueue().CallLater(HandlerAttachedScripted, 0, false, w);
         }
     }
     override void HandlerDeattached(Widget w)
     {
         if (m_Root) m_Root.RemoveComponent(this);
     }
 };