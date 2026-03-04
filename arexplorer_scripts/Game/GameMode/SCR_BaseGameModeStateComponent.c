 [ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Base for game mode state component handlers.", visible: false)]
 class SCR_BaseGameModeStateComponentClass : SCR_BaseGameModeComponentClass
 {
 }

 class SCR_BaseGameModeStateComponent : SCR_BaseGameModeComponent
 {
     //------------------------------------------------------------------------------------------------
  bool GetAllowControls()
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  float GetDuration()
     {
         return 0;
     }

     //------------------------------------------------------------------------------------------------
  bool CanAdvanceState(SCR_EGameModeState nextState)
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  protected void OnStateEntered();

     //------------------------------------------------------------------------------------------------
  override void OnGameStateChanged(SCR_EGameModeState state)
     {
         super.OnGameStateChanged(state);

         if (state == GetAffiliatedState())
             OnStateEntered();
     }

     //------------------------------------------------------------------------------------------------
  SCR_EGameModeState GetAffiliatedState()
     {
         Debug.Error("Not implemented!");
         return -1;
     }
 }