 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseControllerComponentClass: GameComponentClass
 {
 }

 class BaseControllerComponent: GameComponent
 {
     proto external IEntity GetOwner();
     proto external bool CanMove();
     proto external void SetCanMove(bool canMove);

     // callbacks

     event bool ValidateCanMove() { return true; };
 }
