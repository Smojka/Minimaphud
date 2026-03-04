 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class PerceivableComponentClass: GameComponentClass
 {
 }

 class PerceivableComponent: GameComponent
 {
     proto external EAIUnitType GetUnitType();
     proto external float GetVisualRecognitionFactor();
     proto external float GetIlluminationFactor();
     proto external float GetAmbientLV();
     proto external float GetSoundPower();
     proto external float GetEstimatedVisualSize();
     proto external int GetAimpoints(out notnull array<vector> outPoints);
     proto external int GetAimpointsOfType(out notnull array<ref AimPoint> outPoints, EAimPointType type);
     proto external bool IsDisarmed();
     proto external void SetDisarmed(bool disarmed);

     // callbacks

     event void EOnInit(IEntity owner);
 }
