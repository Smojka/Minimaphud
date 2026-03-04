 class SCR_MenuEditorUIComponent : ScriptedWidgetComponent
 {
     [Attribute()]
     protected string m_sAlwaysShownWidgetName;

     [Attribute()]
     protected string m_sHideableWidgetName;

     protected Widget m_AlwaysShownWidget;
     protected Widget m_HideableWidget;

     //------------------------------------------------------------------------------------------------
     Widget GetAlwaysShownWidget()
     {
         return m_AlwaysShownWidget;
     }

     //------------------------------------------------------------------------------------------------
     Widget GetHideableWidget()
     {
         return m_HideableWidget;
     }

     //------------------------------------------------------------------------------------------------
     override void HandlerAttached(Widget w)
     {
         m_AlwaysShownWidget = w.FindWidget(m_sAlwaysShownWidgetName);
         m_HideableWidget = w.FindWidget(m_sHideableWidgetName);
     }
 }