 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CompartmentAccessComponentClass: GameComponentClass
 {
 }

 class CompartmentAccessComponent: GameComponent
 {
     proto external BaseCompartmentSlot GetCompartment();
     proto external BaseCompartmentSlot FindFreeCompartment(IEntity targetEntity, ECompartmentType compartmentType, bool useReserved = true);
     proto external BaseCompartmentSlot FindCompartmentReservedBy(IEntity targetEntity, IEntity reservedBy);
     proto external bool IsInCompartment();
     proto external bool IsInCompartmentADS();
     proto external bool IsTargetVehicleAccessible(IEntity targetEntity);
     proto external bool CanGetInVehicle(IEntity targetEntity);
     proto external IEntity GetOwner();
     proto external bool CanGetInVehicleViaDoor(IEntity targetEntity, BaseCompartmentSlot targetCompartment, int doorInfoIndex);
     proto external bool GetInVehicle(IEntity targetEntity, BaseCompartmentSlot targetCompartment, int doorInfoIndex);
     proto external bool MoveInVehicle(IEntity targetEntity, BaseCompartmentSlot targetCompartment);
     proto external bool WasLastGettingOutEjecting();
     proto external bool WasLastGettingOutTeleportation();
     proto external void GetTeleportTarget(out vector outMat[4]);
     proto external bool IsGettingIn();
     proto external bool IsGettingOut();
     proto external bool CanGetOutVehicle();
     proto external bool CanGetOutVehicleViaDoor(int doorIndex);
     proto external bool GetOutVehicle(int doorInfoIndex, bool ejects);
     proto external bool MoveOutVehicle(int doorInfoIndex, vector targetTransform[4]);
     proto external bool EjectOutOfVehicle();
     proto external bool CanJumpOutVehicle();
     proto external bool JumpOutVehicle();
     static proto IEntity GetVehicleIn(IEntity entity);

     // callbacks

     event protected void OnCompartmentEntered(IEntity targetEntity, BaseCompartmentManagerComponent manager, int mgrID, int slotID, bool move);
     event protected void OnCompartmentLeft(IEntity targetEntity, BaseCompartmentManagerComponent manager, int mgrID, int slotID, bool move);
 }
