 class SCR_WidgetHelper
 {
     //------------------------------------------------------------------------------------------------
  static void GetAllChildren(notnull Widget widget, notnull out array<ref Widget> widgetArray, bool recursive = false)
     {
         Widget child = widget.GetChildren();
         while (child)
         {
             widgetArray.Insert(child);
             if (recursive)
                 GetAllChildren(child, widgetArray, true);
             child = child.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
     static void RemoveAllChildren(notnull Widget widget)
     {
         while (widget.GetChildren())
         {
             widget.GetChildren().RemoveFromHierarchy();
         }
     }

     //------------------------------------------------------------------------------------------------
     static void ResizeToImage(notnull ImageWidget widget, int imageIndex = 0)
     {
         int x, y;
         widget.GetImageSize(imageIndex, x, y);
         widget.SetSize(x, y);
     }

     //------------------------------------------------------------------------------------------------
  static Widget GetRootWidget(notnull Widget widget)
     {
         WorkspaceWidget topTopParent = widget.GetWorkspace();
         Widget parent;
         while (widget && widget != topTopParent)
         {
             parent = widget;
             widget = parent.GetParent();
         }
         return parent;
     }

     //------------------------------------------------------------------------------------------------
  static Widget GetWidgetOrChild(notnull Widget widget, string widgetName)
     {
         if (widget.GetName() == widgetName)
             return widget;

         return widget.FindAnyWidget(widgetName);
     }
 };

 /*
 class SCR_WidgetHelperT<Class T>
 {
     static T GetScriptedComponent(notnull Widget widget)
     {
         return T.Cast(widget.FindHandler(T));
     }
 };
 // */