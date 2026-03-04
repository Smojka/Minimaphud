 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CarControllerComponent_SA_BClass: VehicleControllerComponent_SAClass
 {
 }

 class CarControllerComponent_SA_B: VehicleControllerComponent_SA
 {
     proto external VehicleWheeledSimulation_SA_B GetWheeledSimulation();
     proto external bool IsThrottleTurbo();
     proto external bool GetHandBrake();
     proto external bool GetPersistentHandBrake();
     proto external void SetPersistentHandBrake(bool newValue);
     proto external bool HasAutomaticGearbox();
     proto external int GetFutureGear();
     proto external int GetCurrentGear();
     proto external VehicleWheeledSimulation_SA_B GetSimulation();
 }
