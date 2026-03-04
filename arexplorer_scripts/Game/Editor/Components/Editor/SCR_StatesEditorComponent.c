 [ComponentEditorProps(category: "GameScripted/Editor", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
 class SCR_StatesEditorComponentClass: SCR_BaseEditorComponentClass
 {
 };

 class SCR_StatesEditorComponent : SCR_BaseEditorComponent
 {
     [Attribute(desc: "Default state set every time other state is finished.", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EEditorState))]
     private EEditorState m_DefaultState;

     private EEditorState m_State;
     private bool m_bIsWaiting;
     private ref ScriptInvoker Event_OnStateChange = new ScriptInvoker;
     private ref ScriptInvoker Event_OnIsWaitingChange = new ScriptInvoker;
     protected MenuBase m_MenuUponWaiting;
     protected bool m_bIsSafeDialog;

     bool IsDefaultState()
     {
         return m_State == m_DefaultState;
     }
     EEditorState GetState()
     {
         return m_State;
     }
     bool SetState(EEditorState state)
     {
         if (state == m_State) return true;

         if (CanSet(state))
         {
             EEditorState statePrev = state;
             m_State = state;
             Event_OnStateChange.Invoke(state, statePrev);
             return true;
         }
         return false;
     }
     void UnsetState(EEditorState state)
     {
         if (m_State == state) SetState(m_DefaultState);
     }
     bool CanSet(EEditorState state)
     {
         if (state == m_DefaultState) return true; //--- Always allow setting default
         switch (state)
         {
             case EEditorState.NONE:
                 return true;

             case EEditorState.SELECTING:
                 return true;

             case EEditorState.MULTI_SELECTING:
                 return (m_State != EEditorState.PLACING && m_State != EEditorState.TRANSFORMING);

             case EEditorState.PLACING:
                 return true;

             case EEditorState.TRANSFORMING:
                 return (m_State != EEditorState.PLACING && m_State != EEditorState.MULTI_SELECTING);
         }
         return false;
     }
     bool SetIsWaiting(bool isWaiting)
     {
         if (isWaiting == m_bIsWaiting)
             return true;

         bool result = true;
         if (isWaiting)
         {
             m_MenuUponWaiting = GetGame().GetMenuManager().GetTopMenu();
         }
         else
         {
             result = (m_bIsSafeDialog || !GetGame().GetMenuManager().IsAnyDialogOpen()) && m_MenuUponWaiting == GetGame().GetMenuManager().GetTopMenu();
             m_MenuUponWaiting = null;
         }

         m_bIsWaiting = isWaiting;
         Event_OnIsWaitingChange.Invoke(isWaiting);
         return result;
     }
     bool IsWaiting()
     {
         return m_bIsWaiting;
     }
     void SetSafeDialog(bool value)
     {
         m_bIsSafeDialog = value
     }

     ScriptInvoker GetOnStateChange()
     {
         return Event_OnStateChange;
     }
     ScriptInvoker GetOnIsWaitingChange()
     {
         return Event_OnIsWaitingChange;
     }

     override void EOnEditorDebug(array<string> debugTexts)
     {
         if (!IsActive()) return;
         debugTexts.Insert(string.Format("Editor State: %1", Type().EnumToString(EEditorState, m_State)));
     }
 };