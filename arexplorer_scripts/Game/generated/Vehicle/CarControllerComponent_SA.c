 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CarControllerComponent_SAClass: VehicleControllerComponent_SAClass
 {
 }

 class CarControllerComponent_SA: VehicleControllerComponent_SA
 {
     proto external VehicleWheeledSimulation_SA GetWheeledSimulation();
     proto external bool IsThrottleTurbo();
     proto external bool GetHandBrake();
     proto external bool GetPersistentHandBrake();
     proto external void SetPersistentHandBrake(bool newValue);
     proto external bool HasAutomaticGearbox();
     proto external int GetFutureGear();
     proto external int GetCurrentGear();
     proto external VehicleWheeledSimulation_SA GetSimulation();
 }
