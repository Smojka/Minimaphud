 [ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Game Mode component responsible for controlling the main game state.")]
 class SCR_GameGameModeStateComponentClass : SCR_BaseGameModeStateComponentClass
 {
 }

 class SCR_GameGameModeStateComponent : SCR_BaseGameModeStateComponent
 {
     [Attribute("0", uiwidget: UIWidgets.Slider, "Time limit of game mode in seconds.", params: "0 864000 1", category: "Game Mode")]
     protected float m_fDuration;

     //------------------------------------------------------------------------------------------------
  override float GetDuration()
     {
         return m_fDuration;
     }

     //------------------------------------------------------------------------------------------------
  override bool CanAdvanceState(SCR_EGameModeState nextState)
     {
         if (m_fDuration > 0.0 && m_pGameMode.GetElapsedTime() >= m_fDuration)
             return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  override bool GetAllowControls()
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
     sealed override SCR_EGameModeState GetAffiliatedState()
     {
         return SCR_EGameModeState.GAME;
     }
 }