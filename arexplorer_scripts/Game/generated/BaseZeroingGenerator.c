 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseZeroingGenerator: ScriptAndConfig
 {
     proto external IEntity GetOwnerEntity();
     proto external BaseSightsComponent GetSights();
     proto external WeaponAnimationComponent GetWeaponAnimationComponent();
     proto external void SetPointInfoGeneration(bool doGenerate);
     proto external void SetProjectileResource(ResourceName resourceName);
 }