 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseVehicleClass: GameEntityClass
 {
 }

 class BaseVehicle: GameEntity
 {
     proto external SCR_DamageManagerComponent GetDamageManager();
     proto external void SetWreckModel(ResourceName newModel);
     proto external bool IsAccessible();
 }
