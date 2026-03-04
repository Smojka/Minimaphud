 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class RespawnSystemComponentClass: GameComponentClass
 {
 }

 class RespawnSystemComponent: GameComponent
 {
     [Obsolete()]
     proto external GenericEntity DoSpawn(string prefab, vector position, vector rotation = "0 0 0");

     // callbacks

     event protected bool OnRplSave(ScriptBitWriter w) { return true; };
     event protected bool OnRplLoad(ScriptBitReader r) { return true; };
     event void OnInit(IEntity owner);
     event GenericEntity RequestSpawn(int playerId) { return null; };
 }
