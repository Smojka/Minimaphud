 [ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Game Mode component responsible for adding a 'post-game' period to the game mode.")]
 class SCR_PostGameGameModeStateComponentClass : SCR_BaseGameModeStateComponentClass
 {
 }

 class SCR_PostGameGameModeStateComponent : SCR_BaseGameModeStateComponent
 {
     [Attribute("1", uiwidget: UIWidgets.CheckBox, "Disables player controls in this state if checked.", category: "Game Mode")]
     protected bool m_bDisableControls;

     //------------------------------------------------------------------------------------------------
  override bool GetAllowControls()
     {
         if (m_bDisableControls)
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
     sealed override SCR_EGameModeState GetAffiliatedState()
     {
         return SCR_EGameModeState.POSTGAME;
     }
 }