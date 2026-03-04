 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ScriptedBaseZeroingGenerator: BaseZeroingGenerator
 {
     event protected void OnInit(IEntity owner, BaseSightsComponent sights);
     event protected void OnUpdate(IEntity owner, BaseSightsComponent sights, float timeSlice);
     event bool WB_GetZeroingData(float weaponAngle, float distance, out vector offset, out vector angles);
     event float WB_CalculateWeaponZeroingAnimationValue(float pitch, float distance, out vector offset, out vector angles);
     event vector WB_GetPivotPoint();
     event bool WB_InitGenerator();
     event void WB_CleanupGenerator();
 }