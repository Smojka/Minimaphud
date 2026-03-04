 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ScriptedSightsComponentClass: SightsComponentClass
 {
 }

 class ScriptedSightsComponent: SightsComponent
 {
     event void OnInit(IEntity owner);
     event void OnSightADSActivated();
     event void OnSightADSPostFrame(IEntity owner, float timeSlice);
     event void OnSightADSDeactivated();
     event float GetADSActivationPercentageScript() { return 1.0; };
     event float GetADSDeactivationPercentageScript() { return 1.0; };
 }
