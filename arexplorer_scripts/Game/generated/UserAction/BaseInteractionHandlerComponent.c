 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseInteractionHandlerComponentClass: GameComponentClass
 {
 }

 class BaseInteractionHandlerComponent: GameComponent
 {
     proto external bool IsInteractionAvailable();
     proto external BaseUserAction GetSelectedAction();
     proto external float GetVisibilityRange();

     // callbacks

     event void OnInit(IEntity owner);
     event void OnFrame(IEntity owner, float timeSlice);
     event void OnAfterPostSimulate(IEntity owner, float timeSlice);
 }
