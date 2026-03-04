 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class PerceptionManagerClass: GenericEntityClass
 {
 }

 class PerceptionManager: GenericEntity
 {
     proto external float GetTime();
     proto void GetAmbientLV(out float outDirectLV, out float outAmbientLV, out float outTotalAmbientLv);
     proto external void RequestUpdateAllTargetsFactions();
 }
