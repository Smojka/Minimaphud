 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ScriptedSightsFOVInfo: SightsFOVInfo
 {
     event protected void OnInit(IEntity owner, BaseSightsComponent sights);
     event protected void OnUpdate(IEntity owner, BaseSightsComponent sights, float timeSlice);
     event protected float GetCurrentFOV() { return 0.0; };
 }