 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 sealed class HitZoneContainerComponent: GameComponent
 {
     proto external IEntity GetOwner();
     proto external HitZone GetDefaultHitZone();
     proto external HitZone GetHitZoneByColliderID(int colliderID);
     proto external int GetHitZonesByColliderIDs(out notnull array<HitZone> outHitZones, notnull array<int> colliderIDs);
     proto external HitZone GetHitZone(string colliderName);
     proto external int GetAllHitZones(out notnull array<HitZone> outHitZones);

     // callbacks

     // --------------------------------------------------------------------------------
     event protected bool OnRplSave(ScriptBitWriter writer) { return true; };
     event protected bool OnRplLoad(ScriptBitReader reader) { return true; };
 }
