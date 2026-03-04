
 class EditorMenuBase: MenuRootBase
 {
     //--- Actions

     MenuBase OpenDialog(ChimeraMenuPreset preset)
     {
         if (GetManager().FindMenuByPreset(preset)) return null; //--- Already opened
         return GetManager().OpenDialog(preset);
     }

     MenuBase OpenMenu(ChimeraMenuPreset preset)
     {
         if (GetManager().FindMenuByPreset(preset)) return null; //--- Already opened
         return GetManager().OpenMenu(preset);
     }

     void CloseSelf()
     {
         GetManager().CloseMenu(this);
     }

     protected void DeleteAllChildWidgets(Widget parentWidget)
     {
         while (parentWidget.GetChildren())
         {
             delete parentWidget.GetChildren();
         }
     }

     //--- Getters

     Widget GetWidgetByName(string widgetName)
     {
         Widget root = GetRootWidget();
         if (!root) return null;

         WorkspaceWidget workspace = GetGame().GetWorkspace();
         if (!workspace) return null;

         return root.FindAnyWidget(widgetName);
     }
 };