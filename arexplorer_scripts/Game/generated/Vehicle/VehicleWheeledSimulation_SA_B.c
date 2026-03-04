 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class VehicleWheeledSimulation_SA_BClass: VehicleBaseSimulationClass
 {
 }

 class VehicleWheeledSimulation_SA_B: VehicleBaseSimulation
 {
     proto external float GetSpeedKmh();
     proto external float GetSteering();
     proto external void SetSteering(float in);
     proto external float GetClutch();
     proto external void SetClutch(float in);
     proto external float GetBrake();
     proto external void SetBreak(float in, bool hb);
     proto external float GetThrottle();
     proto external void SetThrottle(float in);
     proto external int GetGear();
     proto external void SetGear(int in);
     proto external bool IsHandbrakeOn();
     proto external void EngineStart();
     proto external void EngineStop();
     proto external bool EngineIsOn();
     proto external float EngineGetRPMIdle();
     proto external float EngineGetRPMPeakTorque();
     proto external float EngineGetRPMPeakPower();
     proto external float EngineGetRPMMax();
     proto external float EngineGetRPM();
     proto external float EngineGetRPMFeedback();
     proto external float EngineGetLoad();
     proto external float EngineGetPeakTorque();
     proto external float EngineGetPeakTorqueState();
     proto external void EngineSetPeakTorqueState(float peakTorque);
     proto external float EngineGetPeakPower();
     proto external float EngineGetPeakPowerState();
     proto external void EngineSetPeakPowerState(float peakPower);
     proto external float GearboxGetEfficiency();
     proto external float GearboxGetEfficiencyState();
     proto external void GearboxSetEfficiencyState(float efficiency);
     proto external int GearboxGearsCount();
     proto external int GearboxForwardGearsCount();
     proto external int WheelCount();
     proto external int WheelGetIndex(string name);
     proto external vector WheelGetPosition(int wheelIdx, float displacement = 0.0);
     proto external string WheelGetName(int wheelIdx);
     proto external bool WheelHasContact(int wheelIdx);
     proto external GameMaterial WheelGetContactMaterial(int wheelIdx);
     proto external vector WheelGetContactPosition(int wheelIdx);
     proto external vector WheelGetContactNormal(int wheelIdx);
     proto external IEntity WheelGetContactEntity(int wheelIdx);
     proto external EWheelContactLiquidState WheelGetContactLiquidState(int wheelIdx);
     proto external GameMaterial WheelGetContactLiquidMaterial(int wheelIdx);
     proto external vector WheelGetContactLiquidPosition(int wheelIdx);
     proto external float WheelGetRPM(int wheelIdx);
     proto external float WheelGetRadius(int wheelIdx);
     proto external float WheelGetRadiusState(int wheelIdx);
     proto external float WheelSetRadiusState(int wheelIdx, float radius);
     proto external float WheelGetMass(int wheelIdx);
     proto external float WheelGetMassState(int wheelIdx);
     proto external float WheelSetMassState(int wheelIdx, float mass);
     proto external float WheelGetLongitudinalSlip(int wheelIdx);
     proto external float WheelGetLateralSlip(int wheelIdx);
     proto external float WheelGetRollingDrag(int wheelIdx);
     proto external void WheelSetRollingDrag(int wheelIdx, float drag);
     proto external float WheelTyreGetRoughness(int wheelIdx);
     proto external float WheelTyreGetRoughnessState(int wheelIdx);
     proto external void WheelTyreSetRoughnessState(int wheelIdx, float roughness);
     proto external float WheelTyreGetLongitudinalFriction(int wheelIdx);
     proto external float WheelTyreGetLongitudinalFrictionState(int wheelIdx);
     proto external void WheelTyreSetLongitudinalFrictionState(int wheelIdx, float lngFriction);
     proto external float WheelTyreGetLateralFriction(int wheelIdx);
     proto external float WheelTyreGetLateralFrictionState(int wheelIdx);
     proto external void WheelTyreSetLateralFrictionState(int wheelIdx, float latFriction);
     proto external float GetNoiseSteerSensitivity();
     proto external void SetNoiseSteerSensitivity(float newValue);
     proto external float GetRoughnessSensitivity();
     proto external void SetRoughnessSensitivity(float newValue);
 }
