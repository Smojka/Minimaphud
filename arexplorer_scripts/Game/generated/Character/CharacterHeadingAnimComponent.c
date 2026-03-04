 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CharacterHeadingAnimComponentClass: GenericComponentClass
 {
 }

 class CharacterHeadingAnimComponent: GenericComponent
 {
     proto external void AlignPosDirWS(vector pPredictedPos, vector pPredictedDir, vector pTargetPos, vector pTargetDir, bool bSnap = false);
     proto external void AlignPosRotWS(vector pPredictedPos, float pPredictedRot[4], vector pTargetPos, float pTargetRot[4], bool bSnap = false);
     proto external void AlignRotWS(float pPredictedRot[4], float pTargetRot[4], bool bSnap = false);
     proto external void ResetAligning();
     proto external void SetFiltering(float pFilterTimeout, float pMaxVel);
     proto external bool IsAligning();
 }
