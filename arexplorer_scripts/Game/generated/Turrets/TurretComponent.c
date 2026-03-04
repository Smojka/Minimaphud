 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class TurretComponentClass: AimingComponentClass
 {
 }

 class TurretComponent: AimingComponent
 {
     proto external BaseSightsComponent GetSights();
     proto external bool HasMoveableBase();
     proto external PointInfo GetCameraAttachmentSlot();
     proto external bool IsVehicleMounted();
     proto external vector GetAimingAngleExcess(vector tgtPosWorld);
     proto external void SetSights(int index);
     proto external int NextSights();
     proto external void SwitchNextSights();
     proto external void SwitchPrevSights();
 }
