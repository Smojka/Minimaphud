 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CharacterVicinityComponentClass: GameComponentClass
 {
 }

 class CharacterVicinityComponent: GameComponent
 {
     proto external void GetAvailableItems(inout notnull array<IEntity> items);
     proto external void SetItemOfInterest(IEntity item);
     proto external void ManipulationComplete();
     proto external IEntity GetItemOfInterest();
     proto external float GetDiscoveryRadius();

     // callbacks

     event protected void OnUpdate();
 }
