
 class SCR_MouseAreaEditorUIComponent : SCR_BaseEditorUIComponent
 {
     protected bool m_bIsMouseOn;
     protected bool m_bIsUsingMouseAndKeyboard;

     //------------------------------------------------------------------------------------------------
  bool IsMouseOn()
     {
         return m_bIsMouseOn || !m_bIsUsingMouseAndKeyboard;
     }

     //------------------------------------------------------------------------------------------------
     protected void OnInputDeviceIsGamepad(bool isGamepad)
     {
         //--- Cache the value
         m_bIsUsingMouseAndKeyboard = !isGamepad;
     }

     //------------------------------------------------------------------------------------------------
     override bool OnMouseEnter(Widget w, int x, int y)
     {
         m_bIsMouseOn = true;
         return false;
     }

     //------------------------------------------------------------------------------------------------
     override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
     {
         m_bIsMouseOn = false;
         return false;
     }

     //------------------------------------------------------------------------------------------------
     override void HandlerAttachedScripted(Widget w)
     {
         OnInputDeviceIsGamepad(!GetGame().GetInputManager().IsUsingMouseAndKeyboard());
         GetGame().OnInputDeviceIsGamepadInvoker().Insert(OnInputDeviceIsGamepad);
     }

     //------------------------------------------------------------------------------------------------
     override void HandlerDeattached(Widget w)
     {
         GetGame().OnInputDeviceIsGamepadInvoker().Remove(OnInputDeviceIsGamepad);
     }
 }