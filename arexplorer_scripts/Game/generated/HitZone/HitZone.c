 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class HitZone: ScriptAndConfig
 {
     //All non true damage taken will be multiplied by this amount
     proto external float GetBaseDamageMultiplier();
     //returns true if this hitzone has collider nodes.
     proto external bool HasColliderNodes();
     //returns index of collider descriptor attached to collider ID
     proto external int GetColliderDescriptorIndex(int colliderID);
     //Get damage multipler for a type of damage in this hitzone
     proto external float GetDamageMultiplier(EDamageType dmgType);
     //return damage reduction
     proto external float GetDamageReduction();
     //return damage threshold
     proto external float GetDamageThreshold();
     proto external int GetAllColliderNames(out notnull array<string> colliderNames);
     proto external void SetDamageOverTime(EDamageType dmgType, float dps);
     //return damage per second of specified type
     proto external float GetDamageOverTime(EDamageType dmgType);
     //Returns number of collider decriptors
     proto external int GetNumColliderDescriptors();
     //script method wrappers
     proto external HitZoneContainerComponent GetHitZoneContainer();
     //Sets the health of this hitzone. Only works when called from server.
     proto external void SetHealth(float health);
     //Sets the scaled health of this hitzone [0, 1]. Only works when called from server.
     proto external void SetHealthScaled(float health);
     //Sets the max health of this hitzone. Only works when called from server.
     proto external void SetMaxHealth(float maxHealth, ESetMaxHealthFlags flag = ESetMaxHealthFlags.NONE);
     //Gets the current health of this hitzone. Avoid tying game logic to GetHealth, there is no guarantee of synchronization! Tie game logic to hitzone damage state.
     proto external float GetHealth();
     //Gets the current scaled health of this hitzone. Avoid tying game logic to this function, there is no guarantee of synchronization! Tie game logic to hitzone damage state.
     proto external float GetHealthScaled();
     //Returns health % needed to trigger the requested damage state.
     proto external float GetDamageStateThreshold(EDamageState damageState);
     //Returns max health of this hitzone
     proto external float GetMaxHealth();
     //Returns hitzone name
     proto external string GetName();
     //Gets current damage state of the hitzone
     proto external EDamageState GetDamageState();
     //Returns the amount of damage to be received at once needed to be considered critical
     proto external float GetCriticalDamageThreshold();
     //Gets previous damage state of the hitzone
     proto external EDamageState GetPreviousDamageState();
     //Hitzone will handle this amount of damage. Damage will only be applied if called from server. Use DamageManager.HandleDamage when possible, using this skips DamageManager.OnDamage
     proto external void HandleDamage(float damage, int damageType, IEntity instigator);
     //Returns true if its a proxy
     proto external bool IsProxy();
     proto external int GetColliderIDs(out notnull array<int> outIDs);
     //gets collider description
     proto bool TryGetColliderDescription(IEntity owner, int descIndex, out vector transformLS[4], out int boneIndex, out int nodeID);
     //Gets collider description from name
     proto bool TryGetColliderDescriptionFromName(IEntity owner, string colliderName, out vector transformLS[4], out int boneIndex, out int nodeID);

     // callbacks

     event void OnInit(IEntity pOwnerEntity, GenericComponent pManagerComponent);
     event protected void OnHealthSet();
     event protected void OnMaxHealthChanged();
     event protected void OnDamageStateChanged();
     event void OnDamage(notnull BaseDamageContext damageContext);
     event float ComputeEffectiveDamage(notnull BaseDamageContext damageContext, bool isDOT);
 }
