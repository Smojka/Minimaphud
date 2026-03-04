 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class MotorExhaustEffectComponentClass: BaseEffectComponentClass
 {
 }

 class MotorExhaustEffectComponent: BaseEffectComponent
 {
     proto external IEntity GetOwner();
     proto external void TurnOff();
     proto external void TurnOn(IEntity owner);
     proto external EntitySlotInfo GetEffectPosition();
     proto external float GetSignalRpm();
     proto external float GetSignalThrust();
     proto external float GetRpmScaled();

     // callbacks

     event void OnInit(IEntity owner);
     event void OnDelete(IEntity owner);
 }
