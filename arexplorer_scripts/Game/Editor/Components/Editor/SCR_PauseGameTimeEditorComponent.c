 [ComponentEditorProps(category: "GameScripted/Editor", description: "Game simulation manager. Works only with SCR_EditorManagerEntity!", icon: "WBData/ComponentEditorProps/componentEditor.png")]
 class SCR_PauseGameTimeEditorComponentClass: SCR_BaseEditorComponentClass
 {
 };

 class SCR_PauseGameTimeEditorComponent : SCR_BaseEditorComponent
 {
     [Attribute("1", desc: "When enabled, the editor will be paused when opened.")]
     protected bool m_bPauseOnOpen;

     protected ChimeraWorld m_World;

     void TogglePause()
     {
         SetPause(!m_World.IsGameTimePaused());
     }
     void SetPause(bool pause)
     {
         //--- Unpausing game, create a rewind point
         if (!pause)
         {
             SCR_RewindComponent rewindManager = SCR_RewindComponent.GetInstance();
             if (rewindManager && !rewindManager.HasRewindPoint())
                 rewindManager.CreateRewindPoint();
         }

         m_World.PauseGameTime(pause);
     }
     bool IsPaused()
     {
         return m_World.IsGameTimePaused();
     }

     void SetPauseOnOpen(bool pause)
     {
         m_bPauseOnOpen = pause;
     }
     bool IsPauseOnOpen()
     {
         return m_bPauseOnOpen;
     }

     protected void OnSaved(ESaveType type, string fileName)
     {
         if (type == ESaveType.USER)
             SetPause(true);
     }

     override protected void EOnEditorOpen()
     {
         GetGame().GetSaveManager().GetOnSaved().Insert(OnSaved);

         if (m_bPauseOnOpen)
         {
             //--- Pause game time when the editor is opened without a rewind point (only after a delay, to give player camera chance to iniliazed after rewinding)
             SCR_RewindComponent rewindManager = SCR_RewindComponent.GetInstance();
             if (rewindManager && !rewindManager.HasRewindPoint())
                 GetGame().GetCallqueue().CallLater(SetPause, 1, false, true);
         }
     }
     override protected void EOnEditorClose()
     {
         GetGame().GetSaveManager().GetOnSaved().Remove(OnSaved);

         //--- Always unpause the game when leaving the editor
         SetPause(false);
     }
     override protected void EOnEditorInit()
     {
         m_World = GetGame().GetWorld();

         //--- Don't pause on open when disabled by CLI param in the dev version
         if (GetGame().IsDev() && System.IsCLIParam("editorDoNotPauseOnOpen"))
             m_bPauseOnOpen = false;
     }
 }