 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseMoveComponent: BaseProjectileComponent
 {
     proto external void EnableSimulation(IEntity owner);
     proto external void Launch(vector direction, vector parentVelocity, float initSpeedCoef, IEntity projectileEntity, IEntity gunner, IEntity parentEntity, IEntity lockedTarget, IEntity weaponComponent);
     proto external vector GetVelocity();
     proto external float GetRewindDuration();
     proto external void SetRewindDuration(float duration);
 }
