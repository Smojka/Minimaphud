 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CarControllerComponentClass: VehicleControllerComponentClass
 {
 }

 class CarControllerComponent: VehicleControllerComponent
 {
     proto external void Shutdown();
     proto external VehicleWheeledSimulation GetWheeledSimulation();
     proto external bool IsThrottleTurbo();
     proto external bool GetHandBrake();
     proto external bool GetPersistentHandBrake();
     proto external void SetPersistentHandBrake(bool newValue);
     proto external void CancelStart();
     proto external bool HasAutomaticGearbox();
     proto external int GetFutureGear();
     proto external int GetCurrentGear();
     proto external VehicleWheeledSimulation GetSimulation();

     // callbacks

     event void OnEngineStartBegin();
     event void OnEngineStartProgress();
     event void OnEngineStartInterrupt();
     event void OnEngineStartSuccess();
     event void OnEngineStartFail(EVehicleEngineStartFailedReason reason);
     event bool OnBeforeEngineStart();
     event void OnEngineStart();
     event void OnEngineStop();
     event void OnPostInit(IEntity owner);
     event void OnDelete(IEntity owner);
 }
