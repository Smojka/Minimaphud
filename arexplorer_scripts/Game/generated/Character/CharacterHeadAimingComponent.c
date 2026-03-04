 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CharacterHeadAimingComponentClass: AimingComponentClass
 {
 }

 class CharacterHeadAimingComponent: AimingComponent
 {
     proto external CharacterAimingComponent GetCharacterAimingComponent();
     proto external void SetLimitAnglesOverride(float fDown, float fUp, float fLeft, float fRight);
     proto external void ResetLimitAnglesOverride();
     proto external void SetPitchLimitReductionMultiplier(float value);
     proto external vector GetLookAngles();
     proto external void GetLookTransformationMS(int nodeID, EDirectBoneMode mode, inout vector offsetFromHead, inout vector baseRotation, inout vector transformFromBone[4]);
     proto external void GetLookTransformationLS(int nodeID, EDirectBoneMode mode, inout vector offsetFromHead, inout vector baseRotation, inout vector transformFromBone[4]);
 }
