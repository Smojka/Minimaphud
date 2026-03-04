 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseCompartmentManagerComponentClass: GameComponentClass
 {
 }

 class BaseCompartmentManagerComponent: GameComponent
 {
     proto external IEntity GetOwner();
     proto external BaseCompartmentSlot FindCompartment(int compartmentID, int mgrId = -1);
     proto external int GetCompartments(out notnull array<BaseCompartmentSlot> outCompartments);
 }
