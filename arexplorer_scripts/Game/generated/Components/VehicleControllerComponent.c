 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class VehicleControllerComponentClass: CompartmentControllerComponentClass
 {
 }

 class VehicleControllerComponent: CompartmentControllerComponent
 {
     proto external bool CanSwitchSeat();
     static proto EVehicleDrivingAssistanceMode GetDrivingAssistanceMode();
     static proto void SetDrivingAssistanceMode(EVehicleDrivingAssistanceMode mode);
     proto external VehicleBaseSimulation GetBaseSimulation();
     proto external FuelManagerComponent GetFuelManager();
     proto external BaseLightManagerComponent GetLightManager();
     proto external PilotCompartmentSlot GetPilotCompartmentSlot();
     proto external SignalsManagerComponent GetSignalsManager();
     proto external BaseWeaponManagerComponent GetWeaponManager();
     proto external void TryStartEngine();
     proto external bool StartEngine();
     proto external void StopEngine(bool playDriverAnimation = true);
     proto external bool IsEngineOn();
     proto external float GetEngineStartupChance();
     proto external void SetEngineStartupChance(float chance);
     proto external bool GetEngineDrowned();
     proto external void SetEngineDrowned(bool drowned);
 }
