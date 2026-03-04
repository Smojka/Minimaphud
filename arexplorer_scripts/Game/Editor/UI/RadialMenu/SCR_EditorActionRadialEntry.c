 //------------------------------------------------------------------------------------------------
 class SCR_EditorActionRadialEntry : ScriptedSelectionMenuEntry
 {
     private ref SCR_EditorActionData m_ActionData;
     private SCR_BaseActionsEditorComponent m_EditorComponent;
     private vector m_CursorWorldPosition;
     private int m_Flags;

     override void OnPerform(IEntity user, BaseSelectionMenu sourceMenu)
     {
         super.OnPerform(user, sourceMenu);
         m_EditorComponent.ActionPerform(m_ActionData.GetAction(), m_CursorWorldPosition, m_Flags);
     }

     override bool CanBeShownScript(IEntity user, BaseSelectionMenu sourceMenu)
     {
         // Context Action Radial Menu only contains entries that are always shown
         return true;
     }

     override bool CanBePerformedScript(IEntity user, BaseSelectionMenu sourceMenu)
     {
         return m_ActionData.GetCanBePerformed();
     }

     override bool GetEntryNameScript(out string outName)
     {
         outName = m_ActionData.GetAction().GetInfo().GetName();
         return true;
     }

     override bool GetEntryDescriptionScript(out string outDescription)
     {
         outDescription = m_ActionData.GetAction().GetInfo().GetDescription();
         return true;
     }

     override bool GetEntryIconPathScript(out string outIconPath)
     {
         outIconPath = m_ActionData.GetAction().GetInfo().GetIconPath();
         return true;
     }

     void SCR_EditorActionRadialEntry(SCR_EditorActionData actionData, SCR_BaseActionsEditorComponent component, vector cursorWorldPosition, EEditorContextActionFlags flags = 0)
     {
         m_ActionData = actionData;
         m_EditorComponent = component;
         m_CursorWorldPosition = cursorWorldPosition;
         m_Flags = flags;
     }

     void ~SCR_EditorActionRadialEntry()
     {
         m_ActionData = null;
     }
 };