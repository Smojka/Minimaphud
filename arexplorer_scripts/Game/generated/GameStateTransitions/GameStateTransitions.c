 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 sealed class GameStateTransitions
 {
     private void GameStateTransitions();
     private void ~GameStateTransitions();

     static proto bool IsTransitionRequested();
     static proto bool IsTransitionInProgress();
     static proto bool IsTransitionRequestedOrInProgress();
     static proto bool RequestPublicServerTransition(JsonApiStruct config);
     [Obsolete("Not supported!")]
     static proto bool RequestConnectViaIP(string IPAddr);
     static proto bool RequestConnectViaRoom(Room room);
     [Obsolete("Use RequestScenarioChangeTransition instead!")]
     static proto bool RequestMissionChangeTransition(MissionHeader mission);
     [Obsolete("Use RequestScenarioChangeTransition instead!")]
     static proto bool RequestWorldChangeTransition(string worldPath);
     static proto bool RequestScenarioChangeTransition(string resourceStr, string addonList);
     [Obsolete("Use RequestScenarioRestart instead!")]
     static proto bool RequestServerReload();
     static proto bool RequestScenarioRestart();
     static proto bool RequestServerConfigChange(notnull ServerConfigMeta configMeta);
     static proto void RequestGameplayEndTransition(KickCauseCode code = KickCauseCode.NONE);
     static proto void RequestGameTerminateTransition();
     static proto bool IsFirstMainMenu();
 }
