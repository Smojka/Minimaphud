 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class DamageManagerComponent: HitZoneContainerComponent
 {
     //Enables damage handling. It will only be done if called from server.
     proto external void EnableDamageHandling(bool enable);
     //Returns true if this damage manager and its hitzones can take damage
     proto external bool IsDamageHandlingEnabled();
     proto external float GetMovementDamage();
     proto external float GetAimingDamage();
     proto external void SetMovementDamage(float damage);
     proto external void SetAimingDamage(float damage);
     //Get default hitzone's health [0...1]
     proto external float GetHealthScaled();
     //sets the health scaled to the default HitZone [0, ..., 1]
     proto external bool SetHealthScaled(float health);
     //Returns current amount of health on the default hitzone
     proto external float GetHealth();
     //Returns default hitzone's max health
     proto external float GetMaxHealth();
     // Returns default hitzone's DamageState
     proto external EDamageState GetState();
     //returns true if the default hitzone is in Destroyed damage state
     proto external bool IsDestroyed();
     //Sets the given entity as the last instigator of damage.
     proto external void SetInstigatorEntity(IEntity instigator);
     proto external void SetInstigator(notnull Instigator instigator);
     //Returns last instigator
     proto external notnull Instigator GetInstigator();
     proto external int GetAttachedColliderIDs(out notnull array<int> outAttachedColliderIDs);
     proto external void HandleDamage(notnull BaseDamageContext damageContext);
     proto external bool IsDamagedOverTime(EDamageType dType);
     proto external float GetDamageOverTime(EDamageType dType);
     proto external void RemoveDamageOverTime(EDamageType dType);

     // callbacks

     event bool ShouldOverrideInstigator(notnull Instigator currentInstigator, notnull Instigator newInstigator)  { return true; };
     event GameMaterial OverrideHitMaterial(HitZone struckHitzone);
     event protected void OnDamageOverTimeAdded(EDamageType dType, float dps, HitZone hz);
     event protected void OnDamageOverTimeRemoved(EDamageType dType, HitZone hz);
     event protected void OnDamageStateChanged(EDamageState state);
     event protected void OnPostInit(IEntity owner);
     event protected void OnInit(IEntity owner);
     event protected void OnDelete(IEntity owner);
     event protected void OnFrame(IEntity owner, float timeSlice);
     event protected bool OnContact(IEntity owner, IEntity other, Contact contact);
     event protected void OnDiag(IEntity owner, float timeSlice);
     event protected void OnDamage(notnull BaseDamageContext damageContext);
     event bool HijackDamageHandling(notnull BaseDamageContext damageContext) {return false;};
     event bool ShouldCountAsHit(notnull BaseDamageContext damageContext) {return true;};
 }
