 [ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Game Mode component responsible for adding a 'pre-game' period to the game mode.")]
 class SCR_PreGameGameModeStateComponentClass : SCR_BaseGameModeStateComponentClass
 {
 }

 class SCR_PreGameGameModeStateComponent : SCR_BaseGameModeStateComponent
 {
     [Attribute("30", uiwidget: UIWidgets.Slider, "Duration of this state in seconds or 0 for infinite duration. (Requires manual start)", params: "0 864000 1", category: "Game Mode")]
     protected float m_fDuration;

     [Attribute("1", uiwidget: UIWidgets.CheckBox, "Disables player controls in this state if checked.", category: "Game Mode")]
     protected bool m_bDisableControls;

     //------------------------------------------------------------------------------------------------
  override float GetDuration()
     {
         return m_fDuration;
     }

     //------------------------------------------------------------------------------------------------
  override bool CanAdvanceState(SCR_EGameModeState nextState)
     {
         return m_fDuration > 0.0 && m_pGameMode.GetElapsedTime() >= m_fDuration;
     }

     //------------------------------------------------------------------------------------------------
  override bool GetAllowControls()
     {
         return !m_bDisableControls;
     }

     //------------------------------------------------------------------------------------------------
     sealed override SCR_EGameModeState GetAffiliatedState()
     {
         return SCR_EGameModeState.PREGAME;
     }
 }