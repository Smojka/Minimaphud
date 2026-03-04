 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class RespawnComponentClass: GameComponentClass
 {
 }

 class RespawnComponent: GameComponent
 {
     proto external void NotifySpawn(IEntity spawnedEntity);
     proto external PlayerController GetPlayerController();

     // callbacks

     event protected void OnInit(IEntity owner);
     event protected void OnDelete(IEntity owner);
     event protected void OnPostInit(IEntity owner);
     event protected void OnDiag(IEntity owner, float timeSlice);
     event protected void OnFrame(IEntity owner, float timeSlice);
 }
