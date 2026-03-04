 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class AimingComponentClass: BaseAimingComponentClass
 {
 }

 class AimingComponent: BaseAimingComponent
 {
     proto external vector GetInitAiming();
     proto external void SetAimingRotationWanted(vector inRotation);
     proto external void SetAimingRotation(vector inRotation);
     proto external vector GetAimingRotationWanted();
     proto external vector GetAimingRotation();
     proto external vector GetAimingDirection();
     proto external vector GetAimingDirectionWorld();
     proto external vector GetAimingRotationModification();
     proto external vector GetModifiedAimingTranslation();
     proto external vector GetCurrentRecoilTranslation();
     proto void GetAimingLimits(out vector limitsHoriz, out vector limitsVert);
     proto external vector GetRawAimingTranslation();
     proto external vector GetRawAimingRotation();
 }
