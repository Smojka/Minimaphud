 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ExtBaseCompartmentSlot: ScriptAndConfig
 {
     proto external int GetAddUserActions(out notnull array<CompartmentUserAction> outAddUserActions);
     proto external IEntity GetOwner();
     proto external BaseControllerComponent GetController();
     proto external ECompartmentType GetType();
     // returns door index with entry position that is closest to provided point (in world space)
     proto external int PickDoorIndexForPoint(vector point);
     proto external IEntity AttachedOccupant();
     proto external IEntity GetOccupant();
     proto external UIInfo GetUIInfo();
     proto external bool IsReserved();
     proto external bool IsReservedBy(IEntity ent);
     proto external void SetReserved(IEntity ent);
     proto external EntitySlotInfo GetPassengerPointInfo();
     proto external bool GetForceFreeLook();
     proto external bool GetMouseSteering();
     // Otherwise they will stay in the compartment and play the unconscious animation.
     proto external bool ShouldEjectUnconsciousDeadCharacters();
     proto external int GetCompartmentMgrID();
     proto external int GetCompartmentSlotID();
     proto external CompartmentDoorInfo GetDoorInfo(int doorIndex);
     proto external bool IsGetInLockedFor(IEntity entity);
     proto external bool IsGetInLocked();
     proto external CompartmentUserAction GetGetInAction();
     proto external CompartmentUserAction GetGetOutAction();
     proto external CompartmentUserAction GetJumpOutAction();
     proto external CompartmentUserAction GetSwitchSeatAction();
     proto external vector GetPosition();

     // callbacks

     // Called on DiagDebugDrawPosition
     event protected void DebugDrawPosition();
 }
