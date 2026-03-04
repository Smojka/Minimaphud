 //------------------------------------------------------------------------------------------------
 class SCR_EditorModeRadialEntry : ScriptedSelectionMenuEntry
 {
     private SCR_EditorModeEntity m_EditorModeEntity;
     private SCR_EditorManagerEntity m_EditorManagerEntity;

     override void OnPerform(IEntity user, BaseSelectionMenu sourceMenu)
     {
         super.OnPerform(user, sourceMenu);
         m_EditorManagerEntity.SetCurrentMode(m_EditorModeEntity.GetModeType());
     }

     override bool CanBeShownScript(IEntity user, BaseSelectionMenu sourceMenu)
     {
         // Only contains entries that are always shown
         return true;
     }

     override bool CanBePerformedScript(IEntity user, BaseSelectionMenu sourceMenu)
     {
         return true;
     }

     override bool GetEntryNameScript(out string outName)
     {
         SCR_UIInfo modeInfo = m_EditorModeEntity.GetInfo();
         if (modeInfo) outName = modeInfo.GetName();
         return true;
     }

     override bool GetEntryDescriptionScript(out string outDescription)
     {
         outDescription = string.Empty;
         return false;
     }

     override bool GetEntryIconPathScript(out string outIconPath)
     {
         SCR_UIInfo modeInfo = m_EditorModeEntity.GetInfo();
         if (modeInfo) outIconPath = modeInfo.GetIconPath();
         return true;
     }

     void SCR_EditorModeRadialEntry(SCR_EditorModeEntity editorModeEntity, SCR_EditorManagerEntity editorManager)
     {
         m_EditorModeEntity = editorModeEntity;
         m_EditorManagerEntity = editorManager;
     }
 }