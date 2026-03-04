 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ScriptedWeaponAimModifier: BaseWeaponAimModifier
 {
     //------------------------------------------------------------------------------------------------
     event protected void OnInit(IEntity weaponEnt);
     event protected void OnActivated(IEntity weaponOwner);
     event protected void OnDeactivated(IEntity weaponOwner);
     event protected void OnCalculate(IEntity owner, WeaponAimModifierContext context, float timeSlice, out vector translation, out vector rotation, out vector turnOffset);
     // called when weapon is fired
     event protected void OnWeaponFired();
 }
