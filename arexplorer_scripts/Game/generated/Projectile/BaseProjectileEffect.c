 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseProjectileEffect: ScriptAndConfig
 {
     proto external bool HasFinished();
     proto external void SetHasFinished(bool value);
     proto external bool IsEnabled();
     proto external void SetIsEnabled(bool value);

     // callbacks

     event void OnInit(IEntity owner);
     event void OnEffect(IEntity pHitEntity, inout vector outMat[3], IEntity damageSource, notnull Instigator instigator, string colliderName, float speed);
     event void OnFrame(IEntity owner, float timeSlice);
     event bool ShouldTick();
 }
