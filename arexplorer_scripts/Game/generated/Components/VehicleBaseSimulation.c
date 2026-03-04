 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class VehicleBaseSimulationClass: GameComponentClass
 {
 }

 class VehicleBaseSimulation: GameComponent
 {
     proto external bool HasAnyGroundContact();
     proto external vector GetGForce();
     proto external float GetGForceMagnitude();
     proto external bool IsValid();

     // callbacks

     event protected void OnInit(IEntity owner);
 }
