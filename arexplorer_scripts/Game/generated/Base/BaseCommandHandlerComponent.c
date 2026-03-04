 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseCommandHandlerComponentClass: GameComponentClass
 {
 }

 class BaseCommandHandlerComponent: GameComponent
 {
     proto external void SetSimulationDisabled(bool pSimulationDisabled);

     // callbacks

     event protected void OnInit(IEntity owner);
     event protected void OnCommandActivate(int pCmdId);
     event protected void OnCommandDeactivate(int pCmdId);
     event protected void Update(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished);
 }
