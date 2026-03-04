 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class VehicleHelicopterSimulationClass: VehicleBaseSimulationClass
 {
 }

 class VehicleHelicopterSimulation: VehicleBaseSimulation
 {
     proto external void SetThrottle(float in);
     proto external void EngineStart();
     proto external void EngineStop();
     proto external bool EngineIsOn();
     proto external int RotorCount();
     proto external void RotorSetForceScaleState(int rotorIdx, float forceScale);
     proto external void RotorSetTorqueScaleState(int rotorIdx, float torqueScale);
     proto external void RotorGetTransform(int rotorIdx, out vector transform[4]);
     proto external void RotorGetAxis(int rotorIdx, out vector axis);
     proto external float RotorGetRadius(int rotorIdx);
     proto external SpinDirection RotorGetSpinDirection(int rotorIdx);
     proto external float RotorGetRPMTarget(int rotorIdx);
     proto external float RotorGetRPM(int rotorIdx);
     proto external RotorState RotorGetState(int rotorIdx);
     proto external LandingGearType LandingGearGetType();
     proto external float GetAltitudeAGL();
 }
