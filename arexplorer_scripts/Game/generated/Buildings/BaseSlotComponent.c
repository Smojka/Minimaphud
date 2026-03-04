 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseSlotComponentClass: GameComponentClass
 {
 }

 class BaseSlotComponent: GameComponent
 {
     proto external EntitySlotInfo GetSlotInfo();
     proto external IEntity GetAttachedEntity();

     // callbacks

     event void OnAttachedEntity(IEntity ent);
     event void OnDetachedEntity(IEntity ent);
 }
