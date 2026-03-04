
 class MenuRootComponent : ScriptedWidgetComponent
 {
     protected Widget m_Widget;
     protected MenuRootBase m_Menu;
     protected ref array<MenuRootSubComponent> m_aComponents = {};

     //------------------------------------------------------------------------------------------------
  static MenuRootComponent GetRootOf(Widget widget)
     {
         if (!widget)
             return null;

         WorkspaceWidget workspace = GetGame().GetWorkspace();
         Widget widgetRoot = widget;
         while (widgetRoot)
         {
             Widget widgetRootParent = widgetRoot.GetParent();
             if (!widgetRootParent || widgetRootParent == workspace)
                 break;
             widgetRoot = widgetRootParent;
         }

         MenuRootComponent rootComponent = MenuRootComponent.Cast(widgetRoot.FindHandler(MenuRootComponent));
         if (!rootComponent)
         {
             rootComponent = new MenuRootComponent;
             widgetRoot.AddHandler(rootComponent);
         }
         return rootComponent;
     }

     //------------------------------------------------------------------------------------------------
  static MenuRootComponent GetRootOf(MenuBase menu)
     {
         Widget widgetRoot = menu.GetRootWidget();
         return GetRootOf(widgetRoot);
     }

     //------------------------------------------------------------------------------------------------
  MenuRootBase GetMenu()
     {
         return m_Menu;
     }

     //------------------------------------------------------------------------------------------------
     Widget GetWidget()
     {
         return m_Widget;
     }

     //------------------------------------------------------------------------------------------------
  void AddComponent(MenuRootSubComponent component)
     {
         if (m_aComponents.Find(component) == -1)
             m_aComponents.InsertAt(component, 0); //--- Place at the beginning to make sure the newest component is found first
     }

     //------------------------------------------------------------------------------------------------
  void RemoveComponent(MenuRootSubComponent component)
     {
         m_aComponents.RemoveItem(component);
     }

     //------------------------------------------------------------------------------------------------
  MenuRootSubComponent FindComponent(typename type, bool showError = false)
     {
         foreach (MenuRootSubComponent component: m_aComponents)
         {
             if (component && component.Type() == type)
                 return component;
         }

         if (showError)
             Print(string.Format("Cannot find UI component '%1'!", type), LogLevel.ERROR);

         return null;
     }

     //------------------------------------------------------------------------------------------------
  int FindComponents(typename type, notnull out array<MenuRootSubComponent> outComponents)
     {
         outComponents.Clear();
         foreach (auto component: m_aComponents)
         {
             if (component && component.Type() == type)
                 outComponents.Insert(component);
         }
         return outComponents.Count();
     }

     //------------------------------------------------------------------------------------------------
     void Init(MenuRootBase menu)
     {
         if (m_Menu)
             return;

         m_Menu = menu;

         int count = m_aComponents.Count();
         foreach (auto component : m_aComponents)
         {
             component.HandlerAttachedScripted(component.GetWidget());

             //--- Show error when the components creates new widgets here, changing the array and breaking indexing
             if (count != m_aComponents.Count())
             {
                 Print(string.Format("Error when initializing MenuRootComponent - new components were added during init of %1! The menu will not function correctly!", component.Type()), LogLevel.ERROR);
                 return;
             }
         }
     }

     //------------------------------------------------------------------------------------------------
     protected void InitNotRoot()
     {
         Init(null);
     }

     //------------------------------------------------------------------------------------------------
     protected void ExecInit(Widget w)
     {
         //--- Recursively iterate through children
         Widget child = w.GetChildren();
         while (child)
         {
             ExecInit(child);
             child = child.GetSibling();
         }

         //--- Iterate through handlers
         int subComponentsCount = w.GetNumHandlers();
         for (int i; i < subComponentsCount; i++)
         {
             MenuRootSubComponent subComponent = MenuRootSubComponent.Cast(w.GetHandler(i));
             if (subComponent)
                 subComponent.HandlerAttachedScripted(w);
         }
     }

     //------------------------------------------------------------------------------------------------
     override void HandlerAttached(Widget w)
     {
         m_Widget = w;
         super.HandlerAttached(w);
         GetGame().GetCallqueue().CallLater(InitNotRoot, 1, false); //--- Initialize components in case the root is not part of MenuRootBase
     }
 };