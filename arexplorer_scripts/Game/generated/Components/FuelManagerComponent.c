 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class FuelManagerComponentClass: GameComponentClass
 {
 }

 class FuelManagerComponent: GameComponent
 {
     proto external int GetFuelNodesList(out notnull array<BaseFuelNode> outNodes);
     proto external bool HasFuel();
     proto external bool CanBeRefueled();
     proto external float GetTotalFuel();
     proto external float GetTotalMaxFuel();

     // callbacks

     event void OnInit(IEntity owner);
 }
