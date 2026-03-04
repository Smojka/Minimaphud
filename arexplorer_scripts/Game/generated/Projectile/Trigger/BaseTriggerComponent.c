 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseTriggerComponent: BaseProjectileComponent
 {
     proto external IEntity GetOwner();
     proto external void OnUserTrigger(IEntity owner);
     //Only used when overriding the instigator is necessary. E.g.: Player A shoots explosive of player B. The instigator needs to be overriden with player A.
     proto external void OnUserTriggerOverrideInstigator(IEntity owner, Instigator instigator);
     proto external bool WasTriggered();
     proto external void SetLive();
     proto external void GetProjectileEffects(typename projectileType, out notnull array<BaseProjectileEffect> outProjectileEffects);

     // callbacks

     event protected void EOnInit(IEntity owner);
     event protected void EOnContact(IEntity owner, IEntity other, Contact contact);
 }
