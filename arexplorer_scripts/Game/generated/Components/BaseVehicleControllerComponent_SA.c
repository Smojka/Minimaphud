 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseVehicleControllerComponent_SAClass: CompartmentControllerComponentClass
 {
 }

 class BaseVehicleControllerComponent_SA: CompartmentControllerComponent
 {
     proto external void Shutdown();
     proto external bool CanSwitchSeat();
     static proto EVehicleDrivingAssistanceMode GetDrivingAssistanceMode();
     static proto void SetDrivingAssistanceMode(EVehicleDrivingAssistanceMode mode);
     proto external VehicleBaseSimulation GetBaseSimulation();
     proto external FuelManagerComponent GetFuelManager();
     proto external BaseLightManagerComponent GetLightManager();
     proto external PilotCompartmentSlot GetPilotCompartmentSlot();
     proto external SignalsManagerComponent GetSignalsManager();
     proto external BaseWeaponManagerComponent GetWeaponManager();
     proto external void CancelStart();
     proto external void TryStartEngine();
     proto external bool StartEngine();
     proto external void ForceStartEngine();
     proto external void ForceStopEngine();
     proto external void StopEngine(bool playDriverAnimation = true);
     proto external bool IsEngineOn();
     proto external float GetEngineStartupChance();
     proto external void SetEngineStartupChance(float chance);
     proto external bool GetEngineDrowned();
     proto external void SetEngineDrowned(bool drowned);

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
