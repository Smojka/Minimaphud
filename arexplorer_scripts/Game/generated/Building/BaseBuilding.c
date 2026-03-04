 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseBuildingClass: StaticModelEntityClass
 {
 }

 class BaseBuilding: StaticModelEntity
 {
     event void OnDamage(float damage, EDamageType type, IEntity pHitEntity, inout vector outMat[3], IEntity damageSource, notnull Instigator instigator, int colliderID, float speed);
 }
