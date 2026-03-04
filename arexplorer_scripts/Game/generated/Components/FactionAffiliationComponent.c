 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class FactionAffiliationComponentClass: GameComponentClass
 {
 }

 class FactionAffiliationComponent: GameComponent
 {
     proto external IEntity GetOwner();
     proto external Faction GetAffiliatedFaction();
     proto external Faction GetDefaultAffiliatedFaction();
     proto external void SetAffiliatedFaction(Faction faction);
     proto external void SetAffiliatedFactionByKey(FactionKey factionKey);

     // callbacks

     event protected void OnPostInit(IEntity owner);
     event protected void OnCompartmentEntering(IEntity vehicle, IEntity occupant, BaseCompartmentSlot compartment, bool move);
     event protected void OnCompartmentEntered(IEntity vehicle, IEntity occupant, BaseCompartmentSlot compartment, bool move);
     event protected void OnCompartmentLeft(IEntity vehicle, IEntity occupant, BaseCompartmentSlot compartment, bool move);
     event protected void OnFactionChanged(Faction previous, Faction current);
 }
