 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
 class SCR_BaseToggleToolbarAction : SCR_EditorToolbarAction
 {
     [Attribute(desc: "When defined, this info will be used when the action is toggled on.")]
     protected ref SCR_UIInfo m_InfoToggled;

     private ref ScriptInvoker m_OnToggleChanged = new ScriptInvoker();

     ScriptInvoker GetOnToggleChange()
     {
         return m_OnToggleChanged;
     }

     protected void Toggle(int value, bool highlight = false)
     {
         m_OnToggleChanged.Invoke(value, highlight);
     }

     void Track()
     {
     }

     void Untrack()
     {
     }

     /*
     Get UI info representing toggled state of the action.
     When not defined, default UI info will be returned.
     \return UI info
     */
     SCR_UIInfo GetInfoToggled()
     {
         if (m_InfoToggled)
             return m_InfoToggled;
         else
             return GetInfo();
     }
 };