 class SCR_WidgetTools
 {
     static Widget FindWidgetInChildren(Widget w, WidgetType type)
     {
         if (w.GetTypeID() == type)
             return w;

         Widget candidate;
         w = w.GetChildren();
         while (w)
         {
             candidate = FindWidgetInChildren(w, type);
             if (candidate)
                 return candidate;

             w = w.GetSibling();
         }
         return null;
     }
     static Widget FindWidgetInChildren(Widget w, typename type)
     {
         if (w.FindHandler(type))
             return w;

         Widget result;
         w = w.GetChildren();
         while (w)
         {
             result = FindWidgetInChildren(w, type);
             if (result)
                 return result;

             w = w.GetSibling();
         }
         return null;
     }
     static Widget FindWidgetInParents(Widget w, WidgetType type)
     {
         while (w)
         {
             if (w.GetTypeID() == type)
                 return w;

             w = w.GetParent();
         }
         return null;
     }

     static ScriptedWidgetEventHandler FindHandlerInChildren(Widget w, typename type)
     {
         ScriptedWidgetEventHandler component = w.FindHandler(type);
         if (component)
             return component;

         w = w.GetChildren();
         while (w)
         {
             component = FindHandlerInChildren(w, type);
             if (component)
                 return component;

             w = w.GetSibling();
         }
         return null;
     }
     static ScriptedWidgetEventHandler FindHandlerInParents(Widget w, typename type)
     {
         ScriptedWidgetEventHandler component;
         while (w)
         {
             component = w.FindHandler(type);
             if (component)
                 return component;

             w = w.GetParent();
         }
         return null;
     }
     static bool InHierarchy(Widget w, Widget parent)
     {
         while (w)
         {
             if (w == parent)
                 return true;

             w = w.GetParent();
         }
         return false;
     }
     static void RemoveChildrenFromHierarchy(Widget w)
     {
         Widget child = w.GetChildren();
         while (child)
         {
             child.RemoveFromHierarchy();
             child = child.GetSibling();
         }
     }
     static MenuBase FindMenu(Widget w)
     {
         if (!w)
             return null;

         MenuManager menuManager = GetGame().GetMenuManager();
         if (!menuManager)
             return null;

         if (!InHierarchy(w, GetGame().GetWorkspace()))
         {
             Debug.Error2("SCR_WidgetTools.GetMenu()", string.Format("Cannot find menu of widget '%1', it's not initialized yet. Are you perhaps calling it too early, e.g., from HandlerAttached()?", w.GetName()));
             return null;
         }

         MenuBase menuCandidate;
         array<ScriptMenuPresetEnum> values = {};
         for (int i, count = SCR_Enum.GetEnumValues(ChimeraMenuPreset, values); i < count; i++)
         {
             menuCandidate = menuManager.FindMenuByPreset(values[i]);
             if (menuCandidate && InHierarchy(w, menuCandidate.GetRootWidget()))
                 return menuCandidate;
         }
         return null;
     }

     static string GetHierarchyLog(Widget w, string delimiter = " / ")
     {
         string log;
         while (w)
         {
             if (log.IsEmpty())
                 log = w.GetName();
             else
                 log = w.GetName() + delimiter + log;

             w = w.GetParent();
         }
         return log;
     }

     static ScriptedWidgetEventHandler FindHandlerOnWidget(Widget root, string widgetName, typename type)
     {
         if (!root)
             return null;

         Widget w = root.FindAnyWidget(widgetName);
         if (!w)
             return null;

         return w.FindHandler(type);
     }
 };